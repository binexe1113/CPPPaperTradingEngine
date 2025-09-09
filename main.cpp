#include "core/portfolio.h"
#include "utils/PriceFetcher.h"

#include "imgui.h"
#include "implot.h"
#include "backends/imgui_impl_glfw.h"
#include "backends/imgui_impl_opengl3.h"

#include <GLFW/glfw3.h>
#include <thread>
#include <atomic>
#include <chrono>
#include <mutex>

std::atomic<bool> running(true);
std::mutex portfolioMutex; // protect portfolio when accessed from UI + thread

void priceUpdater(PriceFetcher& fetcher) {
    while (running) {
        fetcher.updatePrices();
        std::this_thread::sleep_for(std::chrono::milliseconds(100));
    }
}

int main() {
    std::vector<std::string> symbols = {"AAPL", "GOOG", "MSFT", "AMZN", "TSLA"};
    Portfolio portfolio(10000.0);
    PriceFetcher fetcher(symbols);

    // ---- Start price updater thread ----
    std::thread priceThread(priceUpdater, std::ref(fetcher));

    // ---- Setup GLFW + ImGui ----
    if (!glfwInit()) return -1;
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    GLFWwindow* window = glfwCreateWindow(1280, 720, "Paper Trading Engine", nullptr, nullptr);
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

            for (const auto& pos : portfolio.getPositions()) {
                            ImGui::TableNextRow();
                ImGui::TableNextColumn(); ImGui::Text("%s", pos.first.c_str()); // symbol
                ImGui::TableNextColumn(); ImGui::Text("%d", pos.second);        // quantity
                ImGui::TableNextColumn(); ImGui::Text("-");                     // avgPrice not tracked yet! add maybe dont know if useful
            }
                ImGui::EndTable();
            }
            ImGui::End();
        }

        // --- Order Entry Panel ---
        ImGui::Begin("Order Entry");
        ImGui::InputText("Symbol", symbol, sizeof(symbol));
        ImGui::InputInt("Quantity", &quantity);
        if (ImGui::Button("Buy")) { //FIX BUG WHERE CRASHES IF TRY TO BUY AND DONT HAVE MONEY 
            std::lock_guard lock(portfolioMutex);
            portfolio.buyStock(symbol, quantity, fetcher.getPrice(symbol));
        }
        ImGui::SameLine();
        if (ImGui::Button("Sell")) { //FIX BUG WHERE SELL MORE STOCK THAN POSSIBLE AND SMTMS CRASHES
            std::lock_guard lock(portfolioMutex);
            portfolio.sellStock(symbol, quantity, fetcher.getPrice(symbol));
        }
        ImGui::End();

        // --- Prices Panel ---
        ImGui::Begin("Prices");
        for (auto& sym : symbols) {
            ImGui::Text("%s: %.2f", sym.c_str(), fetcher.getPrice(sym));
        }
        ImGui::End();

        //--- Trade Log Panel ---
        { // TO NOT LEAK ONLY 
            std::lock_guard lock(portfolioMutex);
            ImGui::Begin("Trade Log");

            //Table definition
            if (ImGui::BeginTable("trades",5,ImGuiTableFlags_RowBg | ImGuiTableFlags_Borders)){
                ImGui::TableSetupColumn("Action");
                ImGui::TableSetupColumn("Symbol");
                ImGui::TableSetupColumn("Quantity");
                ImGui::TableSetupColumn("Price");
                ImGui::TableSetupColumn("Timestamp");
                ImGui::TableHeadersRow();

                for (const auto& trade: portfolio.getTrades()){
                    ImGui::TableNextRow();
                    ImGui::TableNextColumn(); ImGui::Text("%s", trade.type.c_str());
                    ImGui::TableNextColumn(); ImGui::Text("%s",trade.symbol.c_str());
                    ImGui::TableNextColumn(); ImGui::Text("%d",trade.quantity);
                    ImGui::TableNextColumn(); ImGui::Text("%d",trade.price);
                

            //Table updater
            char bufer[20];
            std::tm* tm = std::localtime(&trade.timestamp);
            std::strftime(bufer,sizeof(bufer),"%H:%M:%S", tm);
            ImGui::TableNextColumn();ImGui::Text("%s",bufer);

                }

                ImGui::EndTable();
            }

            ImGui::End();
        }



                
            

        // --- Render ---
        ImGui::Render();
        int w, h;
        glfwGetFramebufferSize(window, &w, &h);
        glViewport(0, 0, w, h);
        glClearColor(0.1f, 0.1f, 0.12f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
        glfwSwapBuffers(window);
    }

    // ---- Cleanup ----
    running = false;
    priceThread.join();
    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImPlot::DestroyContext();
    ImGui::DestroyContext();
    glfwDestroyWindow(window);
    glfwTerminate();

    return 0;
}
