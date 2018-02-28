#pragma once

namespace RandomTool
{ 

class PrimeSearch
{
	static int s_primeArray[];

	int		m_skip;
	int		m_currentPosition;
	int		m_maxElements;
	int *	m_pCurrentPrime;
	int		m_searches;

public:
	PrimeSearch(int elements);
	int		GetNext(bool restart = false);
	bool	Done() { return (m_searches == *m_pCurrentPrime); }
	void	Restart() { m_currentPosition = 0; m_searches = 0; }
	// change the search size and restart;
	void	InitSizeAndRestart(int elements);
};

} // namespace RandomTool