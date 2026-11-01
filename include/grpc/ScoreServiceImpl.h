#pragma once
#include <score_engine.grpc.pb.h>

#include "core/GameSession.h"

class ScoreServiceImpl final : public scoreengine::ScoreService::Service {
public:
    explicit ScoreServiceImpl(GameSession &session);

    grpc::Status ProcessEvent(
        grpc::ServerContext *context,
        const scoreengine::ScoreEventRequest *request,
        scoreengine::ScoreEventReply *reply
    ) override;

private :
    GameSession &session_;
};
