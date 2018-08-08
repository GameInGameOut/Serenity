#include "FakeLatency.h"

std::deque<CIncomingSequence> sequences;
int32_t lastincomingsequencenumber;

void FakeLatency::AddLatencyToNetchan(INetChannel *netchan, float Latency)
{
	for (auto& seq : sequences)
	{
		if (i_GlobalVars->realtime - seq.curtime >= Latency)
		{
			netchan->m_nInReliableState = seq.inreliablestate;
			netchan->m_nInSequenceNr = seq.sequencenr;
			break;
		}
	}  
}

void FakeLatency::UpdateIncomingSequences()
{
	auto clientState = *reinterpret_cast<uintptr_t*>(uintptr_t(GetModuleHandle("engine.dll")) + 0x586A74); //DONT HARDCODE OFFESTS


	if (!clientState)
		return;
		
	auto intnetchan = *reinterpret_cast<uintptr_t*>(clientState + 0x9C); //Can optimise, already done in CM hook, make a global

	INetChannel* netchan = reinterpret_cast<INetChannel*>(intnetchan);
	if (netchan)
	{
		if (netchan->m_nInSequenceNr > lastincomingsequencenumber)
		{
			//sequences.push_front(netchan->m_nInSequenceNr);
			lastincomingsequencenumber = netchan->m_nInSequenceNr;

			sequences.push_front(CIncomingSequence(netchan->m_nInReliableState, netchan->m_nOutReliableState, netchan->m_nInSequenceNr, i_GlobalVars->realtime));
		}
		if (sequences.size() > 2048)
			sequences.pop_back();
	}

}

FakeLatency* f_FakeLatency;