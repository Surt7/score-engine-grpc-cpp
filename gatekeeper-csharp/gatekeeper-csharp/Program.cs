

using Grpc.Net.Client;
using ScoreEngine.Gatekeeper.Grpc;

Console.WriteLine("=== ScoreEngine Gatekeeper (.NET 10) ===");

using var channel = GrpcChannel.ForAddress("http://localhost:50051");
var client = new ScoreService.ScoreServiceClient(channel);

var eventsToSend = new[]
{
    ("START_BUTTON", 1UL),
    ("BUMPER_1",     2UL),
    ("BUMPER_1",     3UL),
    ("BUMPER_1",     3UL), 
    ("UNKNOWN_TEST", 4UL),
    ("SUPER_BUMPER", 5UL) 
};


foreach (var (switchCode, seqId) in eventsToSend)
{
    var req = new ScoreEventRequest
    {
        SequenceId = seqId,
        SwitchCode = switchCode,
        TimestampNs = DateTimeOffset.UtcNow.ToUnixTimeMilliseconds() * 1_000_000,
        Signature = "VALID_SECRET"
    };
    var res = await client.ProcessEventAsync(req);
    Console.WriteLine($"[C#] {switchCode,-14} (seq: {seqId}) -> Action: {res.Action,-12} | Score: {res.TotalScore} pts | Statut: {res.Status}");
    
    
    await Task.Delay(300);
}

