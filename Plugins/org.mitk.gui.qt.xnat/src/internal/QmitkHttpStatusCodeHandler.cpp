#include "QmitkHttpStatusCodeHandler.h"
#include "QMessageBox"
#include <sstream>
#include <mitkLog.h>

QmitkHttpStatusCodeHandler::QmitkHttpStatusCodeHandler()
{
}

QmitkHttpStatusCodeHandler::~QmitkHttpStatusCodeHandler()
{
}

static void CreateMessageBox(int statusCode, std::string errorMessage)
{
  std::stringstream ss;
  switch(statusCode)
  {
  case 202:
    QMessageBox::warning(nullptr, "HTTP STATUS - 202", "Probably no permission to perform this action.");
    break;
  case 204:
    QMessageBox::warning(nullptr, "204 - NO CONTENT", "Successful request but no content could be returned.");
    break;
  case 301:
    QMessageBox::warning(nullptr, "301 - Moved Permanently", "All requests to this url cannot be processed anymore.");
    break;
  case 400:
    QMessageBox::warning(nullptr, "400 - Bad request", "Server will not respond due to a client error..");
    break;
  case 401:
    QMessageBox::warning(nullptr, "401 - Unauthorized", "Please provide your authentication credentials.");
    break;
  case 403:
    QMessageBox::warning(nullptr, "403 - Forbidden", "You do not have the rights to access this page.");
    break;
  case 404:
    QMessageBox::warning(nullptr, "404 - Not Found", "The requested resource could not be found.");
    break;
  default:
    ss << "An Http Error occured with error code " << statusCode << " and server message: " << errorMessage;
    QMessageBox::warning(nullptr, "HTTP ERROR", ss.str().c_str());
    break;
  }
}

bool QmitkHttpStatusCodeHandler::HandleErrorMessage(const char *_errorMsg)
{
  std::string errorMsg(_errorMsg, strnlen(_errorMsg, strlen(_errorMsg)));
  /*
   * sample error response:
   * ERROR: An error occurred: ctkRuntimeException: Syncing with http request failed. {d55ec279-8a65-46d6-80d3-cec079066109}: 202: Error downloading
   * https:... - server replied: Forbidden
   */


  //TODO: Implement isValid-flag to check if ctkRuntimeExceptio is valid http-exception.
  //TODO: implement re-catching
  //TODO: status-code specific error messages

  if(errorMsg.find("request failed.") == std::string::npos)
    return false;

  std::string::size_type indexOfErrorCode = errorMsg.find(": Error") - 3;
  std::string::size_type indexOfServerResponse = errorMsg.rfind("server replied: ") + 16; //Length of "server replied : " is 16

  if(indexOfErrorCode == std::string::npos || indexOfServerResponse == std::string::npos)
    return false;

  std::string statusCodeString = errorMsg.substr(indexOfErrorCode,3);
  std::stringstream str;
  str << statusCodeString;
  int statusCode;
  str >> statusCode;

  std::string serverResponse = errorMsg.substr(indexOfServerResponse);

  ::CreateMessageBox(statusCode, serverResponse);

  return true;
}
