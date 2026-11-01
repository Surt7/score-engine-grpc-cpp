#include "../../include/grpc/ScoreServiceImpl.h"
#include "../../include/display/ConsoleDisplay.h"
#include <iostream>

ScoreServiceImpl::ScoreServiceImpl(GameSession &session) : session_(session) {
}

grpc::Status ScoreServiceImpl::ProcessEvent(
    grpc::ServerContext *context,
    const scoreengine::ScoreEventRequest *request,
    scoreengine::ScoreEventReply *reply
) {
    (void) context;

    Event event{
        .sequence_id = request->sequence_id(),
        .switch_code = request->switch_code(),
        .timestamp_ns = request->timestamp_ns(),
        .signature = request->signature()
    };

    const auto result = session_.process_event(event);

    ConsoleDisplay::show_event_result(event.switch_code, result);
    ConsoleDisplay::show_json_result(result);

    reply->set_status(result.status == EngineStatus::Accepted ? "Accepted" : "Rejected");
    reply->set_sequence_id(result.sequence_id);
    reply->set_score_delta(result.score_delta);
    reply->set_total_score(result.total_score);
    reply->set_action(std::string(to_string(result.action)));

    return grpc::Status::OK;
}
