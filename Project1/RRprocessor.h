#pragma once
#include"Processor.h"
#include"Scheduler.h"
#include"Process.h"
#include"defs.h"
#include"ProcessQueues.h"
class RRprocessor:public Processor
{
private:
	int counterslice;
	ProcessQueue rdylist;
	ProcessQueue temp;

public:
	RRprocessor(Scheduler* sh):Processor(sh)
	{
	counterslice=0;
	setType(RR);
	}
 void addprocess(Process* p)
{
	if(p!=nullptr)
	{
	rdylist.enqueue(p);

	rdylist.totalreqtime+=p->getCpuTime()-p->getexcuted();



	readynum++;
	}
}

 void peek(Process* p)
 {
	 rdylist.peek(p);
 }

  void printRdyList()
{
	rdylist.print();
}
   void ScheduleAlgo()
{
	
	
	if(Running!=nullptr)
	{
		if (psh->migrate(Running, RR))
		{
			Running = nullptr;
		}
		else 
		{

			Running->incrementexcuted();
			counterslice++;

			if (Running->getexcuted() == Running->getCpuTime())
			{
				psh->Completed(Running);

				counterslice = 0;
			}
			else if (Running->needio() != -1)
			{
				psh->IOHandling(Running);
			}
			
			else
			{
				int timeslice = psh->getslice();

				if (counterslice == timeslice && Running != nullptr)
				{
					counterslice = 0;
					Process* k = Running;
					Running = nullptr;

					addprocess(k);
				}
			}
		}
	}




	Process*p;
	if(Running == nullptr)
	{
		if(!rdylist.isEmpty())
		{
			rdylist.peek(p);
			if(p != nullptr)
			{
				p = dequeueprocess();

				if (!psh->migrate(p, RR))
					Running = p;
			}
		}
	}

};

Process* dequeueprocess()
{
	Process*p;
	rdylist.dequeue(p);
	rdylist.totalreqtime -= p->getCpuTime() - p->getexcuted();
	readynum--;
	return p;
}

 int  gettotalreq()
 {
 
 return rdylist.totalreqtime;
 
 }

 void storeForked(Process* p)
 {
	 Process* q;

	 this->dequeueprocess();
	 temp.enqueue(p);
	 this->peek(q);
	 if (q && q->getForked())
		 storeForked(q);
 }

 void restoreForked()
 {
	 Process* p = nullptr;
	 while (p = this->dequeueprocess())
	 {
		 temp.enqueue(p);
	 }

	 while (!temp.isEmpty())
	 {
		 temp.dequeue(p);
		 this->addprocess(p);
	 }
 }


};