/* This testcase is part of GDB, the GNU debugger.

   Copyright 2008, 2009, 2010 Free Software Foundation, Inc.

   This program is free software; you can redistribute it and/or modify
   it under the terms of the GNU General Public License as published by
   the Free Software Foundation; either version 3 of the License, or
   (at your option) any later version.

   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU General Public License for more details.

   You should have received a copy of the GNU General Public License
   along with this program.  If not, see <http://www.gnu.org/licenses/>.  */

#include <pthread.h>
#include <assert.h>
#include <unistd.h>

static void *
start (void *arg)
{
  while(1) {
    printf("%s\n", __FUNCTION__);
	sleep(1);
  }
  return arg;
}

static void *
end (void *arg)
{
  while(1) {
    printf("%s\n", __FUNCTION__);
	sleep(1);
  }
  return arg;
}

int main(void)
{
  pthread_t thread;
  int i;

      i = pthread_create (&thread, NULL, start, NULL);
      i = pthread_create (&thread, NULL, end, NULL);
    i = pthread_join (thread, NULL);
  return 0;
}
