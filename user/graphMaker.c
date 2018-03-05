#include "user.h"
#include "pstat.h"
#include "param.h"
#include "stat.h"
#include "fs.h"
#include "types.h"

int printInterval = 25;

void spin() {
  int i = 0;
  int j = 0;
  int k = 0;
  for (i = 0; i < 500; i++) {
    for(j = 0; j < 100000; j++) {
      k = j % 10;
      k = k + 1;
    }
  }
}

void print(struct pstat *st)
{
   int i;
   printf(1, "*******PSTAT*******\n");
   for(i = 0; i < NPROC; i++) {
      if (st->inuse[i]) {
          printf(1, "i: %d pid: %d tickets: %d ticks: %d inuse: %d\n", i, st->pid[i], st->tickets[i], st->ticks[i], st->inuse[i]);
      }
   }
}

int main(int argc, char *argv[]) {
	//settickets(30);
	for(int i = 1; i <= 3; i++) {
		//Children spin and die
		if (fork() == 0) {
			settickets(i);
			spin();
			exit();
		}
	}
	//Parent
	struct pstat st;
	getpinfo(&st);
	print(&st);
	printf(1, "Time\t Child1 Ticks\t Child2 Ticks\t Child3 Ticks\n");
	int time = 0;
	//Should have four procs: one parent, three children.
	//Loop for as long as children are alive.
	//Every so many ticks print out how many ticks each child has run for.
	while(st.inuse[3] == 1 && st.inuse[4] == 1 && st.inuse[5] == 1) {
		//Every so many  ticks to reduce work
		if (st.inuse[3] == 0 || st.inuse[4] == 0 || st.inuse[5] == 0)
			printf(1, "Dead");
		if (time % printInterval == 0) {
			printf(1, "%d\t %d\t %d\t %d\n", time, st.ticks[3], st.ticks[4], st.ticks[5]);
			getpinfo(&st);
		}
		time++;
		if (time >= 10000) break;
	}
	//Need to exit with code 0 for reasons.
	exit();
}
