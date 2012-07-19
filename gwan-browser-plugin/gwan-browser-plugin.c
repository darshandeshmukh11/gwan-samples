#define PATH_TO_GWAN "/home/ers/downloads/gwan_linux64-bit/gwan"

#include "gwan-browser-plugin.h"

// http://forum.gwan.com/index.php?p=/discussion/comment/1407/#Comment_1407
// http://devedge-temp.mozilla.org/library/manuals/2002/plugin/1.0/npp_api.html#1004145
// https://developer.mozilla.org/En/Gecko_Plugin_API_Reference:Scripting_plugins

#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <unistd.h>

#define PLUGIN_NAME        "G-WAN Browser plugin"
#define PLUGIN_DESCRIPTION "This plugin embeds G-WAN in a web browser."
#define PLUGIN_VERSION     "1.0.0.0"

static NPNetscapeFuncs *sBrowserFuncs = NULL;

NP_EXPORT(NPError)
NP_Initialize(NPNetscapeFuncs *bFuncs, NPPluginFuncs *pFuncs)
{
  sBrowserFuncs = bFuncs;
  
  *pFuncs = (NPPluginFuncs){
    .newp = NPP_New,
    .destroy = NPP_Destroy,
    .setwindow = NPP_SetWindow,
    .newstream = NPP_NewStream,
    .destroystream = NPP_DestroyStream,
    .asfile = NPP_StreamAsFile,
    .writeready = NPP_WriteReady,
    .write = NPP_Write,
    .print = NPP_Print,
    .event = NPP_HandleEvent,
    .urlnotify = NPP_URLNotify,
    .getvalue = NPP_GetValue,
    .setvalue = NPP_SetValue,
  };

  pid_t pid = vfork();
  
  if (pid == 0) {
    // start and daemonize G-WAN
    int code = execl(
      "/usr/bin/gksu", 
      "gksu", 
      "--description",
      "G-WAN (Global-WAN)",
      PATH_TO_GWAN " -d", 
      NULL
    );
  }
  
  return NPERR_NO_ERROR;
}

NP_EXPORT(NPError)
NP_Shutdown()
{
  pid_t pid = vfork();
  
  if (pid == 0) {
    // stop G-WAN
    int code = execl(
      "/usr/bin/gksu", 
      "gksu", 
      "--description",
      "G-WAN (Global-WAN)",
      PATH_TO_GWAN " -k", 
      NULL
    );
  }
  
  return NPERR_NO_ERROR;
}

NP_EXPORT(char*)
NP_GetPluginVersion()
{
  return PLUGIN_VERSION;
}

NP_EXPORT(const char*)
NP_GetMIMEDescription()
{
  return "application/gwan:gwan:G-WAN browser plugin";
}

NP_EXPORT(NPError)
NP_GetValue(void *future, NPPVariable aVariable, void *aValue)
{
  switch (aVariable)
  {
    case NPPVpluginNameString: {
      *((char**)aValue) = PLUGIN_NAME;
      break;
    }
    
    case NPPVpluginDescriptionString: {
      *((char**)aValue) = PLUGIN_DESCRIPTION;
      break;
    }
    default: {
      return NPERR_INVALID_PARAM;
      break;
    }
  }
  
  return NPERR_NO_ERROR;
}

NPError
NPP_New(NPMIMEType pluginType, NPP instance, uint16_t mode, int16_t argc, char *argn[], char *argv[], NPSavedData *saved)
{
  return NPERR_NO_ERROR;
}

NPError
NPP_Destroy(NPP instance, NPSavedData **save)
{
  return NPERR_NO_ERROR;
}

NPError
NPP_SetWindow(NPP instance, NPWindow *window)
{
  return NPERR_NO_ERROR;
}

NPError
NPP_NewStream(NPP instance, NPMIMEType type, NPStream *stream, NPBool seekable, uint16_t *stype)
{
  return NPERR_GENERIC_ERROR;
}

NPError
NPP_DestroyStream(NPP instance, NPStream *stream, NPReason reason)
{
  return NPERR_GENERIC_ERROR;
}

int32_t
NPP_WriteReady(NPP instance, NPStream *stream)
{
  return 0;
}

int32_t
NPP_Write(NPP instance, NPStream *stream, int32_t offset, int32_t len, void *buffer)
{
  return 0;
}

void
NPP_StreamAsFile(NPP instance, NPStream *stream, const char *fname)
{

}

void
NPP_Print(NPP instance, NPPrint *platformPrint)
{

}

int16_t
NPP_HandleEvent(NPP instance, void *event)
{

  return 0;
}

void
NPP_URLNotify(NPP instance, const char *URL, NPReason reason, void *notifyData)
{

}

NPError
NPP_GetValue(NPP instance, NPPVariable variable, void *value)
{
  return NPERR_GENERIC_ERROR;
}

NPError
NPP_SetValue(NPP instance, NPNVariable variable, void *value)
{
  return NPERR_GENERIC_ERROR;
}
