#include "gwan.h"
#include "stdio.h"

#include "/usr/include/python3.2mu/Python.h"
#pragma link "libraries/libpython3.2mu.so" //or "python3.2"

int init(int argc, char *argv[])
{
  u32 *states = get_env(argv, US_HANDLER_STATES, &states);
  *states = 1 << HDL_AFTER_PARSE;
  
  return 0;
}

static int pyinit = 0;
int main(int argc, char *argv[])
{
  if (!pyinit)
  {
    Py_Initialize(); //crashes if put in init above
    pyinit = 1;
  }
  
  PyRun_SimpleString("print('Hello from Python!')");
  
  return 255;
}

void clean(int argc, char *argv[])
{
  Py_Finalize();
}
