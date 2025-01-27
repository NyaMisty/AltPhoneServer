#!/usr/bin/python3

import re
import sys

def patch_altsign(source_file):
    with open(source_file, 'rb') as f:
        content = f.read()

    content = re.sub(br'L("([^"\\]|\\.)*")', br'U(\1)', content)
    content = content.replace(b'std::wstring', b'std::string')
    content = content.replace(b'boost/filesystem.hpp', b'filesystem')
    content = content.replace(b'boost::filesystem', b'std::filesystem')

    content = content.replace(b'"%FT%T%z"', b'"%Y-%m-%dT%H:%M:%SZ"')
    content = content.replace(b'localtime(', b'gmtime(')

    content = content.replace(b'winsock2.h', b'WinSock2.h')

    with open(source_file, 'wb') as f:
        f.write(content)


def patch_ldid(source_file):
    with open(source_file, 'rb') as f:
        content = f.read()

    content = content.replace(br'\\\\', br'/') # c escape + regex escape = 2*2
    content = content.replace(br'\\', br'/')
    content = re.sub(br'/(\.[A-Za-z])', br'\\\\\1', content)

    content = content.replace(br'int main(', br'int __main(')

    with open(source_file, 'wb') as f:
        f.write(content)


def patch_altserver(source_file):
	str1=br'''
#define IDCANCEL 0
#define MessageBox(x, content, title, xx) (this->ShowAlert(title, content " (Ctrl-C to avoid)"), 1)

// Observes all exceptions that occurred in all tasks in the given range.
template<class T, class InIt>
void observe_all_exceptions(InIt first, InIt last)
{
	// TODO: FIX THIS
}
'''
	str2=br'''
HWND AltServerApp::windowHandle() const
{
	return _windowHandle;
}

HINSTANCE AltServerApp::instanceHandle() const
{
	return _instanceHandle;
}


bool AltServerApp::boolValueForRegistryKey(std::string key) const
{
	return false;
}

void AltServerApp::setBoolValueForRegistryKey(bool value, std::string key)
{
	return;
}

std::string AltServerApp::serverID() const
{
	//auto serverID = GetRegistryStringValue(SERVER_ID_KEY);
	//return serverID;
	return "1234567";
}

pplx::task<std::pair<std::shared_ptr<Account>, std::shared_ptr<AppleAPISession>>> AltServerApp::Authenticate(std::string appleID, std::string password, std::shared_ptr<AnisetteData> anisetteData)
{
	auto verificationHandler = [=](void)->pplx::task<std::optional<std::string>> {
		return pplx::create_task([=]() -> std::optional<std::string> {
			std::cout << "Enter two factor code" << std::endl;
			std::string _verificationCode = "";
			std::cin >> _verificationCode;
			auto verificationCode = std::make_optional<std::string>(_verificationCode);
			_verificationCode = "";

			return verificationCode;
		});
	};

	return pplx::create_task([=]() {
		if (anisetteData == NULL)
		{
			throw ServerError(ServerErrorCode::InvalidAnisetteData);
		}

		return AppleAPI::getInstance()->Authenticate(appleID, password, anisetteData, verificationHandler);
	});
}

void AltServerApp::HandleAnisetteError(AnisetteError& error)
{
	this->ShowAlert("AnisetteData error: ", error.localizedDescription());
}

void AltServerApp::ShowNotification(std::string title, std::string message)
{
	std::cout << "Notify: " << title << std::endl << "    " << message << std::endl;
}


extern "C" int getchar();
void AltServerApp::ShowAlert(std::string title, std::string message)
{
	std::cout << "Alert: " << title << std::endl << "    " << message << std::endl;
	std::cout << "Press any key to continue..." << std::endl;
	//char a;
	//std::cin >> a;
	getchar();
}

fs::path AltServerApp::appDataDirectoryPath() const
{
	fs::path altserverDirectoryPath("./AltServerData");

	if (!fs::exists(altserverDirectoryPath))
	{
		fs::create_directory(altserverDirectoryPath);
	}

	return altserverDirectoryPath;
}

void AltServerApp::Start(HWND windowHandle, HINSTANCE instanceHandle)
{
	ConnectionManager::instance()->Start();

	// DeviceManager only needs 
	const char *isNoUSB = getenv("ALTSERVER_NO_SUBSCRIBE");
	if (!isNoUSB) {
		DeviceManager::instance()->Start();
	}
}

void AltServerApp::Stop()
{
}
'''

	with open(source_file, 'rb') as f:
		content = f.read()

	content = re.sub(br'L("([^"\\]|\\.)*")', br'U(\1)', content)
	content = re.sub(br'\n(std::string StringFromWideString.*?\n\{[\s\S]+?\})', br'/*\1*/', content)
	content = re.sub(br'\n(std::wstring WideStringFromString.*?\n\{[\s\S]+?\})', br'/*\1*/', content)
	content = content.replace(b'std::wstring', b'std::string')
	content = content.replace(b'std::string_convert', b'std::wstring_convert')

	content = content.replace(b'boost/filesystem.hpp', b'filesystem')
	content = content.replace(b'boost::filesystem', b'std::filesystem')

	if source_file.endswith('AltServerApp.cpp'):

		# MessageBox
		# IDCANCEL
		# fs::path AltServerApp::appDataDirectoryPath
		content = content.replace(b'\r', b'')

		content = content.replace(b'#include <windows.h>\n', b'')
		content = content.replace(b'#include <windowsx.h>\n', b'')
		content = content.replace(b'#include <strsafe.h>\n', b'')
		content = content.replace(b'#include <ShlObj_core.h>\n', b'')
		content = content.replace(b'#include <winsparkle.h>\n', b'')
		content = content.replace(b'#pragma comment( lib, "gdiplus.lib" ) \n', b'')
		content = content.replace(b'#include <gdiplus.h> \n', b'')
		content = content.replace(b'#include "resource.h"\n', b'')

		def removePart(content, start, end):
			content = re.sub(br'\n' + start + br'[\S\s]+?(' + end + br')', br'\1', content)
			return content
		content = removePart(content, br'const char\* REGISTRY_ROOT_KEY', br'\nAltServerApp\* AltServerApp::_instance')
		content = removePart(content, br'static int CALLBACK BrowseFolderCallback', br'\npplx::task<std::shared_ptr<Application>> AltServerApp::InstallApplication')
		content = removePart(content, br'\n.*? AltServerApp::Authenticate', br'\npplx::task<std::shared_ptr<Team>> AltServerApp::FetchTeam')
		content = removePart(content, br'void AltServerApp::ShowNotification', br'\nvoid AltServerApp::ShowErrorAlert')
		content = removePart(content, br'bool AltServerApp::CheckDependencies', br'\nfs::path AltServerApp::certificatesDirectoryPath')

		def insertBefore(content, marker, newcontent):
			content = content.replace(marker, newcontent + b'\n' + marker)
			return content
		
		content = insertBefore(content, b'AltServerApp* AltServerApp::_instance = nullptr;', str1)
		content = insertBefore(content, b'fs::path AltServerApp::certificatesDirectoryPath', str2)

	with open(source_file, 'wb') as f:
		f.write(content)


def patch_for_macOS():
    source_file = "upstream_repo/AltServer/ConnectionManager.cpp"
    with open(source_file, 'rb') as f:
        content = f.read()
    content = content.replace(br'int addrlen = sizeof(clientAddress);', br'socklen_t addrlen = sizeof(clientAddress);')
    with open(source_file, 'wb') as f:
        f.write(content)

    source_file = "upstream_repo/AltSign/Archiver.cpp"
    with open(source_file, 'rb') as f:
        content = f.read()
    content = content.replace(b'#include <filesystem>', b'#include <vector>\n#include <filesystem>')
    with open(source_file, 'wb') as f:
        f.write(content)

    source_file = "upstream_repo/AltSign/ProvisioningProfile.cpp"
    with open(source_file, 'rb') as f:
        content = f.read()
    content = content.replace(br'timeval creationDate = { this->_creationDateSeconds, this->_creationDateMicroseconds };',
                              br'timeval creationDate = { this->_creationDateSeconds, static_cast<int>(this->_creationDateMicroseconds) };')
    content = content.replace(br'timeval expirationDate = { this->_expirationDateSeconds, this->_expirationDateMicroseconds };',
                              br'timeval expirationDate = { this->_expirationDateSeconds, static_cast<int>(this->_expirationDateMicroseconds) };')
    with open(source_file, 'wb') as f:
        f.write(content)

# patch now

## patch altsign
altsign_files = ["AltSign/AnisetteData.cpp",
                 "AltSign/AppleAPI+Authentication.cpp",
                 "AltSign/AppleAPI.cpp",
                 "AltSign/AppleAPI.hpp",
                 "AltSign/Archiver.cpp",
                 "AltSign/Certificate.cpp",
                 "AltSign/ProvisioningProfile.cpp",
                  ]
for f in altsign_files:
    patch_altsign("upstream_repo/" + f)

## path ldid
patch_ldid("upstream_repo/ldid/ldid.cpp")

## patch altserver
server_files = ["AltServer/AltServer.cpp",
                "AltServer/AltServerApp.cpp",
                "AltServer/AltServerApp.h",
                "AltServer/AnisetteDataManager.cpp",
                "AltServer/ClientConnection.cpp",
                "AltServer/ConnectionManager.cpp",
                "AltServer/DeveloperDiskManager.cpp",
                "AltServer/DeviceManager.cpp",
]
for f in server_files:
    patch_altserver("upstream_repo/" + f)

## patch build
patch_for_macOS()
