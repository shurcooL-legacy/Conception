#include "Main.h"

Thread::Thread(GLFWthreadfun pFunction, void * pArgument, std::string sName)
	: m_bCanStart(false),
	  m_bShouldBeRunning(true),
	  m_bThreadEnded(false),
	  m_sName(sName),
	  ///m_pFpsCounter(FpsCounter::CreateCounter(sName))
	  m_pArguments(),
	  m_oThread(-1)
{
	m_pArguments[0] = this;
	m_pArguments[1] = pArgument;
	m_oThread = glfwCreateThread(pFunction, m_pArguments);
	if (IsAlive())
		printf("%s thread (tid = %d) created.\n", m_sName.c_str(), m_oThread);
	else {
		printf("Couldn't create %s thread.\n", m_sName.c_str());
		throw 0;
	}

	//m_bCanStart = true;		// Allow the thread to start
}

Thread::~Thread()
{
	if (IsAlive())
	{
		RequestStop();

		glfwWaitThread(m_oThread, GLFW_WAIT);
		m_oThread = -1;
		std::cout << m_sName << " thread has been shut down.\n";
	}

	///FpsCounter::RemoveCounter(m_pFpsCounter);
}

// Allow the thread to start
void Thread::Start()
{
	std::cout << m_sName << " thread is starting.\n";
	m_bCanStart = true;
}

void Thread::ThreadEnded()
{
	m_bThreadEnded = true;
	std::cout << m_sName << " thread has ended.\n";
}

/*FpsCounter * Thread::GetFpsCounter()
{
	return m_pFpsCounter;
}*/

Thread * Thread::GetThisThreadAndRevertArgument(void *& pArgument)
{
	void ** pArguments = static_cast<void **>(pArgument);

	Thread * pThread = static_cast<Thread *>(pArguments[0]);
	pArgument = pArguments[1];

	// Delay the thread from starting until the end of its Thread object constructor
	while (!pThread->m_bCanStart && pThread->ShouldBeRunning())
		glfwSleep(0.001);

	return pThread;
}
