#pragma once
#include "SDK.h"

class FakeLatency
{
public:
	void UpdateIncomingSequences();
	void AddLatencyToNetchan(INetChannel *netchan, float Latency);
}; extern FakeLatency* f_FakeLatency;

struct CIncomingSequence
{
	CIncomingSequence::CIncomingSequence(int instate, int outstate, int seqnr, float time)
	{
		inreliablestate = instate;
		outreliablestate = outstate;
		sequencenr = seqnr;
		curtime = time;
	}
	int inreliablestate;
	int outreliablestate;
	int sequencenr;
	float curtime;
};
    