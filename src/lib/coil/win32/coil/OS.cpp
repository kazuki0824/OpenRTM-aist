﻿// -*- C++ -*-
/*!
 * @file OS_win32.cpp
 * @brief OS class
 * @date $Date$
 * @author Nobuhiko Miyamoto<n-miyamoto@aist.go.jp>
 *
 * Copyright (C) 2019
 *     Robot Innovation Research Center,
 *     Intelligent Systems Research Institute,
 *     National Institute of
 *         Advanced Industrial Science and Technology (AIST), Japan
 *     All rights reserved.
 *
 * $Id$
 *
 */


#include <coil/OS.h>
#include <string>


namespace coil
{
  osversion::osversion() : major(0), minor(0)
    {

    }
  osversion::osversion(const osversion& obj) : major(obj.major), minor(obj.minor)
    {

    }
  osversion::osversion(DWORD majar_version,DWORD minor_version) : major(majar_version), minor(minor_version)
    {
    }

  /*!
  * @if jp
  *
  * @brief RtlGetVersion APIによりOSの情報取得
  * RtlGetVersion関数によりOSのメジャーバージョン、マイナーバージョン、
  * ビルドナンバー、サービスパックの情報を取得する
  * RtlGetVersion関数アドレス取得失敗、OSの情報取得失敗の場合は-1を返す
  * この関数はrtlgetinfo関数で使用するためのものであり、基本的に外部からは使用しない
  *
  *
  * @param name 構造体名称
  *
  * @return 0: 成功, -1: 失敗
  *
  * @else
  *
  * @brief Obtain OS information using RtlGetVersion API
  * Get major version, minor version, build number, service pack information of OS by RtlGetVersion function
  * RtlGetVersion function Returns -1 in the case of address acquisition failure, OS information acquisition failure.
  *
  * @param name Name of structure
  *
  * @return 0: Successful, -1: failed
  *
  * @endif
  */
  bool rtlgetinfo(utsname* name)
  {
      HMODULE handle = GetModuleHandle("ntdll.dll");

      if (handle)
      {
          typedef LONG(WINAPI* RtlGetVersionFunc)(PRTL_OSVERSIONINFOW lpVersionInformation);

          RTL_OSVERSIONINFOW version_info;
          RtlGetVersionFunc RtlGetVersion = (RtlGetVersionFunc)GetProcAddress(handle, "RtlGetVersion");
          if (RtlGetVersion != nullptr)
          {
              if (RtlGetVersion(&version_info) == 0)
              {
                  const char *os;
                  if (version_info.dwPlatformId == VER_PLATFORM_WIN32_NT)
                  {
                      os = "Windows NT %d.%d";
                  }
                  else
                  {
                      os = "Windows CE %d.%d";
                  }

                  sprintf_s(name->release, sizeof(name->release), os,
                      static_cast<int>(version_info.dwMajorVersion),
                      static_cast<int>(version_info.dwMinorVersion));

                  sprintf_s(name->version, sizeof(name->release), "Build %d %ls",
                      static_cast<int>(version_info.dwBuildNumber),
                      version_info.szCSDVersion);

                  return true;
              }
          }
          FreeLibrary(handle);
      }
      return false;
  }

  /*!
   * @if jp
   *
   * @brief システム情報取得
   *
   * システム情報を構造体に設定して返す。
   * OSのバージョンはRtlGetVersionにより取得する
   * RtlGetVersionはRTL_OSVERSIONINFOWにOSのメジャーバージョン、
   * マイナーバージョン、ビルドナンバー、サービスパックの情報などを格納する
   * ただし、ライブラリ(ntdll.dll)が存在せず利用できない場合は
   * VerifyVersionInfoによるバージョン比較を行う。
   * VerifyVersionInfoは設定したOSのバージョンと指定のバージョンを比較する関数である。
   * VerifyVersionInfoを使う場合はビルドナンバーは設定されない
   *
   * @param name 構造体名称
   *
   * @return 0: 成功, -1: 失敗
   *
   * @else
   *
   * @brief Get System information
   *
   * Return a system information to a structure.
   * Set system information to structure and return it.
   * Obtain version of OS by RtlGetVersion function.
   * RtlGetVersion function stores the major version, minor version, build number, service pack information etc of OS in RTL_OSVERSIONINFOW.
   * However, if the library (ntdll.dll) does not exist and can not be used, version comparison by VerifyVersionInfo function is performed.
   * VerifyVersionInfo function is a function to compare the version of the specified OS with the specified version.
   * When using VerifyVersionInfo function, the build number is not set.
   *
   * @param name Name of structure
   *
   * @return 0: Successful, -1: failed
   *
   * @endif
   */
  int uname(utsname* name)
  {
    if (name == NULL) return 0;
    int ret(0);

    // name.sysname
    ::strcpy_s(name->sysname, sizeof(name->sysname), "Win32");


    
    if(!rtlgetinfo(name))
    {

        std::map<std::string, osversion> oslist;
        oslist["Windows 2000"] = osversion(5, 0);
        oslist["Windows XP 32bit"] = osversion(5, 1);
        oslist["Windows XP 64bit"] = osversion(5, 2);
        oslist["Windows Vista"] = osversion(6, 0);
        oslist["Windows 7"] = osversion(6, 1);
        oslist["Windows 8"] = osversion(6, 2);
        oslist["Windows 8.1"] = osversion(6, 3);
        oslist["Windows 10"] = osversion(10, 0);

        for(auto & o : oslist)
        {
            // name.release, name.version
            OSVERSIONINFOEX version_info;
            ULONGLONG condition = 0;
            version_info.dwOSVersionInfoSize = sizeof(OSVERSIONINFOEX);
            version_info.dwMajorVersion = o.second.major;
            version_info.dwMinorVersion = o.second.minor;
            VER_SET_CONDITION(condition, VER_MAJORVERSION, VER_EQUAL);
            VER_SET_CONDITION(condition, VER_MINORVERSION, VER_EQUAL);

            if (::VerifyVersionInfo(&version_info, VER_MAJORVERSION | VER_MINORVERSION, condition))
            {
                const char *os;
                if (version_info.dwPlatformId == VER_PLATFORM_WIN32_NT)
                {
                    os = "Windows NT %d.%d";
                }
                else
                {
                    os = "Windows CE %d.%d";
                }

                sprintf_s(name->release, sizeof(name->release), os,
                    static_cast<int>(version_info.dwMajorVersion),
                    static_cast<int>(version_info.dwMinorVersion));

                break;
            }


        }
    }


    // name.machine
    SYSTEM_INFO sys_info;
    GetSystemInfo(&sys_info);
    WORD arch = sys_info.wProcessorArchitecture;
    char cputype[COIL_UTSNAME_LENGTH/2];
    char subtype[COIL_UTSNAME_LENGTH/2];

    switch (arch)
      {
      case PROCESSOR_ARCHITECTURE_INTEL:
        strcpy_s(cputype, sizeof(cputype), "x86 Intel");
        if (sys_info.wProcessorLevel == 3)
          strcpy_s(subtype, sizeof(subtype), "80386");
        else if (sys_info.wProcessorLevel == 4)
          strcpy_s(subtype, sizeof(subtype), "80486");
        else if (sys_info.wProcessorLevel == 5)
          strcpy_s(subtype, sizeof(subtype), "Pentium");
        else if (sys_info.wProcessorLevel == 6)
          strcpy_s(subtype, sizeof(subtype), "Pentium Pro");
        else if (sys_info.wProcessorLevel == 7)
          strcpy_s(subtype, sizeof(subtype), "Pentium II");
        else
          strcpy_s(subtype, sizeof(subtype), "Pentium Family");
        break;
      case PROCESSOR_ARCHITECTURE_MIPS:
          strcpy_s(cputype, sizeof(cputype), "MIPS");
          strcpy_s(subtype, sizeof(subtype), "");
          break;
      case PROCESSOR_ARCHITECTURE_ALPHA:
          strcpy_s(cputype, sizeof(cputype), "ALPHA");
          strcpy_s(subtype, sizeof(subtype), "");
          break;
      case PROCESSOR_ARCHITECTURE_PPC:
          strcpy_s(cputype, sizeof(cputype), "PPC");
          strcpy_s(subtype, sizeof(subtype), "");
          break;
      case PROCESSOR_ARCHITECTURE_SHX:
          strcpy_s(cputype, sizeof(cputype), "SHX");
          strcpy_s(subtype, sizeof(subtype), "");
          break;
      case PROCESSOR_ARCHITECTURE_ARM:
          strcpy_s(cputype, sizeof(cputype), "ARM");
          strcpy_s(subtype, sizeof(subtype), "");
          break;
      case PROCESSOR_ARCHITECTURE_IA64:
          strcpy_s(cputype, sizeof(cputype), "IA64");
          strcpy_s(subtype, sizeof(subtype), "");
          break;
      case PROCESSOR_ARCHITECTURE_ALPHA64:
          strcpy_s(cputype, sizeof(cputype), "ALPHA64");
          strcpy_s(subtype, sizeof(subtype), "");
          break;
      case PROCESSOR_ARCHITECTURE_MSIL:
          strcpy_s(cputype, sizeof(cputype), "MSIL");
          strcpy_s(subtype, sizeof(subtype), "");
          break;
      case PROCESSOR_ARCHITECTURE_AMD64:
          if (sys_info.wProcessorLevel == 6)
          {
              strcpy_s(cputype, sizeof(cputype), "x64 Intel");
              strcpy_s(subtype, sizeof(subtype), "Pentium Pro");
          }
          else
          {
              strcpy_s(cputype, sizeof(cputype), "x64(AMD or Intel)");
              strcpy_s(subtype, sizeof(subtype), "");
          }
          break;
      case PROCESSOR_ARCHITECTURE_IA32_ON_WIN64:
          strcpy_s(cputype, sizeof(cputype), "IA32_ON_WIN64");
          strcpy_s(subtype, sizeof(subtype), "");
          break;
      case PROCESSOR_ARCHITECTURE_NEUTRAL:
          strcpy_s(cputype, sizeof(cputype), "NEUTRAL");
          strcpy_s(subtype, sizeof(subtype), "");
          break;
#ifdef PROCESSOR_ARCHITECTURE_ARM64
      case PROCESSOR_ARCHITECTURE_ARM64:
          strcpy_s(cputype, sizeof(cputype), "ARM64");
          strcpy_s(subtype, sizeof(subtype), "");
          break;
#endif
#ifdef PROCESSOR_ARCHITECTURE_ARM32_ON_WIN64
      case PROCESSOR_ARCHITECTURE_ARM32_ON_WIN64:
          strcpy_s(cputype, sizeof(cputype), "ARM32_ON_WIN64");
          strcpy_s(subtype, sizeof(subtype), "");
          break;
#endif
#ifdef PROCESSOR_ARCHITECTURE_IA32_ON_ARM64
      case PROCESSOR_ARCHITECTURE_IA32_ON_ARM64:
          strcpy_s(cputype, sizeof(cputype), "IA32_ON_ARM64");
          strcpy_s(subtype, sizeof(subtype), "");
          break;
#endif
      default:
        strcpy_s(cputype, sizeof(cputype), "Unknown");
        strcpy_s(subtype, sizeof(subtype), "Unknown");
      }
    sprintf_s(name->machine, sizeof(name->machine), "%s %s", cputype, subtype);

    // name.nodename
    DWORD len = COIL_UTSNAME_LENGTH;
    if (GetComputerNameExA(ComputerNameDnsHostname,
                            name->nodename,
                            &len) == false)
    ret = -1;
    return ret;
  }

  /*!
   * @if jp
   *
   * @brief 呼び出し元プロセスのプロセスID取得
   *
   * 呼び出し元プロセスのプロセスIDを返す。
   *
   * @return プロセスID
   *
   * @else
   *
   * @brief Get process ID of the caller process
   *
   * Return a process ID of the caller process.
   *
   * @return Process ID
   *
   * @endif
   */
  typedef int pid_t;
  pid_t getpid()
  {
    return ::_getpid();
  }

  /*!
   * @if jp
   *
   * @brief 呼び出し元プロセスの親プロセスのプロセスID取得
   *
   * 呼び出し元プロセスの親プロセスのプロセスIDを返す。
   *
   * @return 0
   *
   * @else
   *
   * @brief Get process ID of the parent process
   *
   * Return a process ID of the parent process.
   *
   * @return 0
   *
   * @endif
   */
  pid_t getppid()
  {
    return 0;
  }

  /*!
   * @if jp
   *
   * @brief 環境変数取得
   *
   * 環境変数を返す。
   *
   * @param name 環境変数名称
   *
   * @return 環境変数の値(NULL: 該当なし)
   *
   * @else
   *
   * @brief Get environment variable
   *
   * Return a environment variable.
   *
   * @param name Name of environment variable
   *
   * @return Value of environment variable(NULL: nonexistent)
   *
   * @endif
   */
  bool getenv(const char* name, std::string &env)
  {
    size_t return_size;
    ::getenv_s(&return_size, NULL, 0, name);
    if (return_size == 0)
    {
        return false;
    }
    char *buff = new char[return_size * sizeof(char)];
    ::getenv_s(&return_size, buff, return_size, name);
    env = buff;
    return true;
  }


  GetOpt::GetOpt(int argc, char* const argv[], const char* opt, int flag)
    : optind(1), opterr(1), optopt(0),
      m_argc(argc), m_argv(argv), m_opt(opt), m_flag(flag)
  {
    this->optarg = coil::optarg;
    coil::optind = 1;
  }

  GetOpt::~GetOpt()
  {
    coil::optind = 1;
  }


  int GetOpt::operator()()
  {
    coil::opterr = opterr;
    coil::optind = optind;

    int result = getopt(m_argc, m_argv, m_opt);

    optarg = coil::optarg;
    optind = coil::optind;
    optopt = coil::optopt;
    return result;
  }
} // namespace coil
