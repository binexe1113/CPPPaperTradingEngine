#include "core/portfolio.h"

#include "symbols/tickerloader.h"

#include "api/yhfinance.h"

#include "utils/PriceFetcher.h"

#include "imgui.h"

#include "implot.h"

#include "backends/imgui_impl_glfw.h"

#include "backends/imgui_impl_opengl3.h"

#include <GLFW/glfw3.h>

#include <iostream>

#include <thread>

#include <atomic>

#include <ctime>

#include <chrono>

#include <mutex>

std::atomic < bool > running(true);
std::mutex portfolioMutex; // protect portfolio when accessed from UI + thread

void priceUpdater(PriceFetcher & fetcher) {
    while (running) {
        fetcher.updatePrices();
        std::this_thread::sleep_for(std::chrono::milliseconds(200));
    }
}

int main() {
    YahooFinanceAPI yahoo;

    auto symbols = loadTickers("../../symbols/ibov_yahoo.csv");
    //FOR DEBUG//
    {
        std::cout << "Loaded" << symbols.size() << "symbols\n";
        //for (auto& s:symbols)
        //std::cout << s << std::endl;
    }


    std::thread updateThread(&YahooFinanceAPI:: priceUpdater, &yahoo, symbols, 100);//method, class, seconds for delay

    Portfolio portfolio(10000.0);
    PriceFetcher fetcher(symbols);


    // ---- Setup GLFW + ImGui ----
    if (!glfwInit()) return -1;
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    GLFWwindow * window = glfwCreateWindow(1280, 720, "Paper Trading Engine", nullptr, nullptr);
    glfwMakeContextCurrent(window);
    glfwSwapInterval(1);

    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImPlot::CreateContext();
    ImGui::StyleColorsDark();
    ImGui_ImplGlfw_InitForOpenGL(window, true);
    ImGui_ImplOpenGL3_Init("#version 330");

    // ---- GUI State ----
    static char symbol[16] = "AAPL";
    static int quantity = 1;

    while (!glfwWindowShouldClose(window) && running) {
        glfwPollEvents();
        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();

        // --- Portfolio Panel ---
        {
            std::lock_guard lock(portfolioMutex);
            ImGui::Begin("Portfolio");
            ImGui::Text("Cash: $%.2f", portfolio.getCash());
            ImGui::Separator();

            if (ImGui::BeginTable("positions", 3)) {
                ImGui::TableSetupColumn("Symbol");
                ImGui::TableSetupColumn("Quantity");
                ImGui::TableSetupColumn("Avg Price");
                ImGui::TableHeadersRow();

                for (const auto & pos: portfolio.getPositions()) {
                    ImGui::TableNextRow();
                    ImGui::TableNextColumn();
                    ImGui::Text("%s", pos.first.c_str()); // symbol
                    ImGui::TableNextColumn();
                    ImGui::Text("%d", pos.second); // quantity
                    ImGui::TableNextColumn();
                    ImGui::Text("-"); // avgPrice not tracked yet! add maybe dont know if useful
                }
                ImGui::EndTable();
            }
            ImGui::End();
        }

        // --- Order Entry Panel ---
        ImGui::Begin("Order Entry");
        static int selectedSymbolIndex = 0;
        // Make sure current index is valid
        if (selectedSymbolIndex >= (int) symbols.size()) {
            selectedSymbolIndex = 0;
        }

        if (ImGui::BeginCombo("Symbol", symbols[selectedSymbolIndex].c_str())) {
            for (int n = 0; n < (int) symbols.size(); n++) {
                bool isSelected = (selectedSymbolIndex == n);
                if (ImGui::Selectable(symbols[n].c_str(), isSelected)) {
                    selectedSymbolIndex = n;
                }
                if (isSelected) {
                    ImGui::SetItemDefaultFocus();
                }
            }
            ImGui::EndCombo();
        }

        ImGui::InputInt("Quantity", & quantity);

        if (ImGui::Button("Buy")) { // FIX BUG: crashes if not enough money
            std::lock_guard lock(portfolioMutex);
            portfolio.buyStock(symbols[selectedSymbolIndex], quantity, fetcher.getPrice(symbols[selectedSymbolIndex]));
        }

        ImGui::SameLine();

        if (ImGui::Button("Sell")) { // FIX BUG: selling more stock than owned
            std::lock_guard lock(portfolioMutex);
            portfolio.sellStock(symbols[selectedSymbolIndex], quantity, fetcher.getPrice(symbols[selectedSymbolIndex]));
        }

        ImGui::End();

        // --- Prices Panel ---
        ImGui::Begin("Prices");
        for (auto & sym: symbols) {
            ImGui::Text("%s: %.2f", sym.c_str(), fetcher.getPrice(sym));
        }
        ImGui::End();

        //--- Trade Log Panel ---
        { // TO NOT LEAK ONLY 
            std::lock_guard lock(portfolioMutex);
            ImGui::Begin("Trade Log");

            //Table definition
            if (ImGui::BeginTable("trades", 5, ImGuiTableFlags_RowBg | ImGuiTableFlags_Borders)) {
                ImGui::TableSetupColumn("Action");
                ImGui::TableSetupColumn("Symbol");
                ImGui::TableSetupColumn("Quantity");
                ImGui::TableSetupColumn("Price");
                ImGui::TableSetupColumn("Timestamp");
                ImGui::TableHeadersRow();

                for (const auto & trade: portfolio.getTrades()) {
                    ImGui::TableNextRow();
                    ImGui::TableNextColumn();
                    ImGui::Text("%s", trade.type.c_str());
                    ImGui::TableNextColumn();
                    ImGui::Text("%s", trade.symbol.c_str());
                    ImGui::TableNextColumn();
                    ImGui::Text("%d", trade.quantity);
                    ImGui::TableNextColumn();
                    ImGui::Text("%.2f", trade.price);

                    //Table updater
                    char bufer[20];
                    std::tm * tm = std::localtime( & trade.timestamp);
                    std::strftime(bufer, sizeof(bufer), "%H:%M:%S", tm);
                    ImGui::TableNextColumn();
                    ImGui::Text("%s", bufer);

                }

                ImGui::EndTable();
            }

            ImGui::End();
        }

        // --- Price Chart Panel ---
        ImGui::Begin("Price Chart");

        // Symbol selector (separate from Order Entry)
        static int selectedGraphIndex = 0;
        if (ImGui::BeginCombo("Symbol", symbols[selectedGraphIndex].c_str())) {
            for (int n = 0; n < (int) symbols.size(); n++) {
                bool isSelected = (selectedGraphIndex == n);
                if (ImGui::Selectable(symbols[n].c_str(), isSelected)) {
                    selectedGraphIndex = n;
                }
                if (isSelected) {
                    ImGui::SetItemDefaultFocus();
                }
            }
            ImGui::EndCombo();
        }

        // Retrieve history for selected symbol
        const auto & hist = fetcher.getHistory(symbols[selectedGraphIndex]);
        if (!hist.empty()) {
            std::vector < double > xs, ys;
            xs.reserve(hist.size());
            ys.reserve(hist.size());

            for (auto & pt: hist) {
                xs.push_back((double) pt.timestamp); // X = time_t
                ys.push_back(pt.price); // Y = price
            }

            if (ImPlot::BeginPlot("Price vs Time", ImVec2(-1, 300))) {
                ImPlot::SetupAxes("Time", "Price");
                ImPlot::SetupAxisScale(ImAxis_X1, ImPlotScale_Time); // interpret x as time_t
                ImPlot::PlotLine(symbols[selectedGraphIndex].c_str(), xs.data(), ys.data(), (int) xs.size());
                ImPlot::EndPlot();
            }
        }
       // std::cout<<"NO PRICES TO GRAPH"<<std::endl;

        ImGui::End();

        // --- Render ---
        ImGui::Render();
        int w, h;
        glfwGetFramebufferSize(window, & w, & h);
        glViewport(0, 0, w, h);
        glClearColor(0.1f, 0.1f, 0.12f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
        glfwSwapBuffers(window);
    }

    // ---- Cleanup ----
    running = false;
    updateThread.join();
    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImPlot::DestroyContext();
    ImGui::DestroyContext();
    glfwDestroyWindow(window);
    glfwTerminate();

    return 0;
}