////////////////////////////////////////////////////////////////////////////////
//																			  //
//	File: loopTimerSDL.cpp													  //
//																			  //
//	Original author: Tomas Sandnes / NITH / 2007-2008						  //
//	Changed by:      ---													  //
//																			  //
//	Descr.: A helper class for gameLoop timing.								  //
//																			  //
//	Special notes: Requires the SDL library.								  //
//																			  //
////////////////////////////////////////////////////////////////////////////////



// I N C L U D E S
// ===============

#include "loopTimerSDL.h"

namespace PG330Helpers
{

// P U B L I C   F U N C T I O N S
// ===============================

CLoopTimer::CLoopTimer(	unsigned short usWantedUpdatesPerInterval,     // Defaults to 60.
						unsigned short usIntervalTimeInMilliSeconds )  // Defaults to 1000.
{
	// Calling own functions instead of setting variables directly to verify valid input.
	SetUpdatesPerInterval(usWantedUpdatesPerInterval);
	SetIntervalDuration(usIntervalTimeInMilliSeconds);

	// Setting the rest of the variables to default values.
	m_usUpdatesLastInt = 0;
	m_usUpdatesCurrInt = 0;
	m_ui32CurrIntStartTick = SDL_GetTicks();
	m_ui32NextIntStartTick = m_ui32CurrIntStartTick + m_usIntDur;
	m_ui32NextUpdateTick = m_ui32CurrIntStartTick + ((m_usUpdatesCurrInt + 1) * m_usIntDur / m_usWantedUPI);

}  // END CLoopTimer



CLoopTimer::~CLoopTimer()
{
}  // END ~CLoopTimer

unsigned short CLoopTimer::getUpdatesPerIntervall()
{
	return m_usWantedUPI;
}

void CLoopTimer::SetUpdatesPerInterval( unsigned short usWantedUPI ) 
{ 
	if ((1 > usWantedUPI))
	{
		m_usWantedUPI = 1;
	}
	else
	{
		m_usWantedUPI = usWantedUPI;
	}

}  // END SetUpdatesPerInterval



void CLoopTimer::SetIntervalDuration( unsigned short usIntDurInMilliSecs ) 
{ 
	if ((1 > usIntDurInMilliSecs))
	{
		m_usIntDur = 1;
	}
	else
	{
		m_usIntDur = usIntDurInMilliSecs;
	}

}  // END SetIntervalDuration



// Call Start() when you start using the timers (when you enter the gameloop, or resume play after a pause).
// Also call it whenever you have updated the "Updates per Interval" or "Interval Duration" variables, 
// to activate the change immediately.
void CLoopTimer::Start()
{
	m_usUpdatesLastInt = 0;
	m_usUpdatesCurrInt = 0;
	m_ui32CurrIntStartTick = SDL_GetTicks();
	m_ui32NextIntStartTick = m_ui32CurrIntStartTick + m_usIntDur;
	m_ui32NextUpdateTick = m_ui32CurrIntStartTick + ((m_usUpdatesCurrInt + 1) * m_usIntDur / m_usWantedUPI);

}  // END Start



// This function makes the program (or current thread, if multi-threaded) halt execution until its 
// time for another update. In other words, it frees up the CPU for other tasks until its time for 
// a new update. 
// NOTE: As with any kind of "sleep" functionality, we're not guaranteed to get control back at the 
//       exact moment we want it, that's up to the OS (Windows) to handle. (99.9% of the time it 
//       works like a charm though. :-)
void CLoopTimer::SleepUntilUpdate()
{
	// Sleep until it's time for coming update.
	long lTicksRemaining = m_ui32NextUpdateTick - SDL_GetTicks();
	if (0 < lTicksRemaining)
	{
		SDL_Delay(lTicksRemaining);
	}

	// Sleep done - update variables so they're ready for the next update again.
	++m_usUpdatesCurrInt;
	m_ui32NextUpdateTick = m_ui32CurrIntStartTick + ((m_usUpdatesCurrInt + 1) * m_usIntDur / m_usWantedUPI);
	while (m_ui32NextUpdateTick > m_ui32NextIntStartTick)
	{
		m_ui32CurrIntStartTick = m_ui32NextIntStartTick;
		m_ui32NextIntStartTick += m_usIntDur;
		m_usUpdatesLastInt = m_usUpdatesCurrInt;
		m_usUpdatesCurrInt = 0;
	}

}  // END SleepUntilUpdate



// Returns true if it's time for a new update (unlike "SleepUntilUpdate()", above, this one 
// doesn't call any sleep functionality. Its merely a check to see if enough time has passed 
// since last update.)
bool CLoopTimer::IsTimeForUpdate()
{
	bool r_bDoUpdate = false;

	if (SDL_GetTicks() >= m_ui32NextUpdateTick)
	{
		r_bDoUpdate = true;

		// Update variables so they're ready for the next update again.
		++m_usUpdatesCurrInt;
		m_ui32NextUpdateTick = m_ui32CurrIntStartTick + ((m_usUpdatesCurrInt + 1) * m_usIntDur / m_usWantedUPI);
		while (m_ui32NextUpdateTick > m_ui32NextIntStartTick)
		{
			m_ui32CurrIntStartTick = m_ui32NextIntStartTick;
			m_ui32NextIntStartTick += m_usIntDur;
			m_usUpdatesLastInt = m_usUpdatesCurrInt;
			m_usUpdatesCurrInt = 0;
		}
	}

	return r_bDoUpdate;

}  // END IsTimeForUpdate

} // namespace PG330Helpers