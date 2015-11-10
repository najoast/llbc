# llbc
Low Level Base Compoment
主要由于个人时间问题,llbc库当前基本没有任何文档(库相对庞大,写doc是一个比写代码更累的事),此Readme.txt也是非常简单的一个说明,实际编译安装及使用中,你可能会遇到一些问题,可以及时email给我:lailongwei@126.com,后期会逐步完善文档及支持,望见谅.
库当前主要在"游爱"及"37"公司的手游项目中使用.
====================================================================================
llbc为一套轻量级的跨平台基础开发库,提供了跨平台开发中的所有基础支持(包括字符串处理,日志,线程,配置,Timer,ObjRef,网络及其它所有基础支持),在最顶层,llbc使用Service的概念将这些所有支持以compoments的方式集成,也就意味着,在开发你的应用的时候,只需要一个Service就拥有所有你需要的功能.

主要目录说明:
/llbc: 核心库,使用c++语言编码
/testsuite: 核心库测试套件
/wrap: 其它语言封装库
/wrap/pyllbc: 针对python语言的封装

另:
  主要由于个人时间问题,文档基本没有,但testsuite中有足够的用例供大家参考,wrap/pyllbc/testsuite中也有足够的python用例可供参考.

====================================================================================
编译:
长期测试维护平台为win/linux平台,Mac,iOS,Android主要因为个人时间问题,难以长期维护(Android基本未编译测试,虽然代码中有此平台的兼容代码,Mac及iOS平台之前有过简单的编译测试)
llbc库没有任何依赖可以直接安装,json代码文件使用jsoncpp,已经作调整,以支持64位int,并整合到库代码中.
Windows:
  当前只在vs2008上编译测试过,其它版本vs编译应该没有什么问题,你如果使用的是其它版本的vs,遇到问题欢迎email.
Linux:
  请确认是否已经安装libuuid(sudo yum install libuuid-devel)
  make all & make install
Mac:
  请确认是否已经安装xCode command line tools
  make all & make install(untested)
iOS:
  使用xCode编译即可(untested)
  
====================================================================================
安装:
Windows:
  llbc: 编译出的dll所在目录llbc/Release目录,copy使用即可
  testsuite: 编译出的测试套件在llbc/Release目录,直接使用即可
  wrap/pyllbc: python封装使用了自动化脚本,编译出来的python库将只有一个.pyd,目录在wrap/pyllbc/lib
  注:
    如果你需要编译64位版本的llbc或者pyllbc,请在项目中调整你的编译配置即可,对于64位版本的pyllbc,请确认你机器使用的python为64位版本的python.
    如果你需要编译debug版本的llbc或者pyllbc,同样也在编译配置中调整即可.
    
Linux & mac:
  llbc: 编译出的so所在目录: llbc/lib,可以直接copy使用
  testsuite 编译出的可执行程序目录: testsuite/bin,直接使用
  pyllbc: 编译出的so所在目录为: wrap/pyllbc/lib,可以直接copy到你的python版本的site-package目录使用
  注:
    linux版本的makefile集成了install指令,建议使用make install安装
    
iOS:
  xCode编译出来dylib,直接在你的项目中引用即可
  
Android:
  没时间处理Android平台,暂时没有安装说明
  
  
====================================================================================
// llbc基础Demo(c++):

#include "llbc.h"
using namespace llbc;

int main(int argc, char *argv[])
{
  std::cout <<"MajorVersion: " <<LLBC_majorVersion <<std::endl;
  std::cout <<"MinorVersion: " <<LLBC_minorVersion <<std::endl;
  std::cout <<"UpdateNumber: " <<LLBC_updateNumber <<std::endl;
  std::cout <<"IsDebugVersion: " <<LLBC_isDebugVer <<std::endl;
  
  std::cout <<"VersionInfo: " <<std::endl;
  std::cout <<LLBC_GetVersionInfo(true) <<std::endl;
  
  return 0;
}

====================================================================================
// llbc Service Demo(c++): 参考testsuite/comm/TestCase_Comm_Svc.h/.cpp

=====================================================================================
# pyllbc基础Demo(python):
import llbc

print 'llbc information:'

print 'info(non verbose):', llbc.GetVersionInfo(False)
print 'info(verbose):'
print llbc.GetVersionInfo(True)

print 'major version:', llbc.MajorVersion
print 'minor version:', llbc.MinorVersion
print 'update num:', llbc.UpdateNo
print 'is debug version:', llbc.Debug

print 'machine endian:', llbc.Endian.MachineEndian

=====================================================================================
# pyllbc Service Demo(python): 参考wrap/pyllbc/testsuite/communication/testcase_svcbase.py
