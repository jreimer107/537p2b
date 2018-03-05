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


int main(int argc, char *argv[]) {
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
	printf(1, "Time\t Child1 Ticks\t Child2 Ticks\t Child3 Ticks\t");
	int time = 0;
	//Should have four procs: one parent, three children.
	//Loop for as long as children are alive.
	//Every so many ticks print out how many ticks each child has run for.
	while(st.inuse[1] && st.inuse[2] && st.inuse[3]) {
		//Every so many  ticks to reduce work
		if (time % printInterval == 0) {
			printf(1, "%d\t %d\t %d\t %d\t", time, st.ticks[1], st.ticks[2], st.ticks[3]);
			getpinfo(&st);
			time += printInterval;
		}
	}
	//Need to exit with code 0 for reasons.
	exit();
}
