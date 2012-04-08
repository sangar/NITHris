////////////////////////////////////////////////////////////////////////////////
//																			  //
//	File: loopTimerSDL.h													  //
//																			  //
//	Original author: Tomas Sandnes / NITH / 2007-2008						  //
//	Changed by:      ---													  //
//																			  //
//	Descr.: A helper class for gameLoop timing.								  //
//																			  //
//	Special notes: Requires the SDL library.								  //
//																			  //
////////////////////////////////////////////////////////////////////////////////

#ifndef LOOP_TIMERSDL_H  // Inclusion guard.
#define LOOP_TIMERSDL_H



// I N C L U D E S
// ===============

#include <SDL.h>
namespace PG330Helpers
{


// C L A S S   D E C L A R A T I O N
// =================================

class CLoopTimer
{
public:
	// Constructor.
	CLoopTimer( unsigned short usWantedUpdatesPerInterval = 60, 
				unsigned short usIntervalTimeInMilliSeconds = 1000 );
	unsigned short getUpdatesPerIntervall();
	// Destructor.
	~CLoopTimer();
	// Set number of wanted updates during the time interval.
	void SetUpdatesPerInterval( unsigned short usWantedUPI );
	// Set duration (in millisecs) of the time interval.
	void SetIntervalDuration( unsigned short usIntDurInMilliSecs );
	// Call Start() when you start using the timers (when you enter the gameloop, or resume play after a pause).
	// Also call it whenever you have updated the "Updates per Interval" or "Interval Duration" variables, 
	// to activate the change immediately.
	void Start();  
	// This function makes the program (or current thread, if multi-threaded) halt execution until its 
	// time for another update. In other words, it frees up the CPU for other tasks until its time for 
	// a new update. 
	// NOTE: As with any kind of "sleep" functionality, we're not guaranteed to get control back at the 
	//       exact moment we want it, that's up to the OS (Windows) to handle. (99.9% of the time it 
	//       works like a charm though. :-)
	void SleepUntilUpdate();  
	// Returns true if it's time for a new update (unlike "SleepUntilUpdate()", above, this one 
	// doesn't call any sleep functionality. Its merely a check to see if enough time has passed 
	// since last update.)
	bool IsTimeForUpdate();  

private:
	unsigned short	m_usWantedUPI;			 // Number of updates we're striving to get per interval.
	unsigned short	m_usIntDur;				 // Duration of interval in milliseconds.
	unsigned short	m_usUpdatesLastInt;		 // Number of updates last interval total.
	unsigned short	m_usUpdatesCurrInt;		 // Number of updates current interval so far.
	Uint32			m_ui32CurrIntStartTick;	 // When current interval started.
	Uint32			m_ui32NextIntStartTick;	 // When next interval starts.
	Uint32			m_ui32NextUpdateTick;	 // Ideal time for next update.
};

} // namespace PG330Helpers

#endif  // LOOP_TIMERSDL_H
