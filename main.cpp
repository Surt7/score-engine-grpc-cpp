#include <iostream>
#include <nlohmann/json.hpp>
#include "core/GameSession.h"
#include "grpc/ScoreServiceImpl.h"
#include <grpcpp/grpcpp.h>
#include <memory>


int main() {
    GameSession session;
    session.init();

    ScoreServiceImpl service(session);


    const std::string server_address = "0.0.0.0:50051";

    grpc::ServerBuilder builder;

    builder.AddListeningPort(server_address, grpc::InsecureServerCredentials());
    builder.RegisterService(&service);


    std::unique_ptr<grpc::Server> server(builder.BuildAndStart());
    std::cout << "=== ScoreEngine gRPC ecoute sur " << server_address << " ===\n";


    std::cout << "ScoreEngine pret. Appuyez sur Entree pour arreter et voir le bilan...\n";
    std::cin.get();

    server->Shutdown();

    session.display_summary();

    return 0;
}
