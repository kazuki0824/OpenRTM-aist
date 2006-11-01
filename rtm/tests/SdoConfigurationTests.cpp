// -*- C++ -*-
/*!
 * @file 
 * @brief Properties test class
 * @date $Date: 2006-11-01 11:23:35 $
 * @author Shinji Kurihara
 * $Id: SdoConfigurationTests.cpp,v 1.1 2006-11-01 11:23:35 kurihara Exp $
 *
 */

/*
 * $Log: not supported by cvs2svn $
 */

#include <cppunit/ui/text/TestRunner.h>
#include <cppunit/TextOutputter.h>
#include <cppunit/extensions/TestFactoryRegistry.h>
#include <cppunit/extensions/HelperMacros.h>
#include <cppunit/TestAssert.h>
#include <vector>
#include <string>

#include "../SdoConfiguration.h"

using namespace SDOPackage;
using namespace std;

int g_argc;
vector<string> g_argv;

// Configuration_impl クラスのテスト
class SdoConfigurationTests : public CppUnit::TestFixture
{
  CPPUNIT_TEST_SUITE(SdoConfigurationTests);
  CPPUNIT_TEST(test_set_device_profile);
  CPPUNIT_TEST(test_set_service_profile);
  CPPUNIT_TEST(test_add_organization);
  CPPUNIT_TEST(test_remove_service_profile);
  CPPUNIT_TEST(test_remove_organization);
  CPPUNIT_TEST(test_get_configuration_parameters);
  //  CPPUNIT_TEST(test_get_configuration_parameter_values);
  //  CPPUNIT_TEST(test_get_configuration_parameter_value);
  //  CPPUNIT_TEST(test_set_configuration_parameter);
  CPPUNIT_TEST(test_get_configuration_sets);
  //  CPPUNIT_TEST(test_get_configuration_set);
  //  CPPUNIT_TEST(test_set_configuration_set_values);
  //  CPPUNIT_TEST(test_get_active_configuration_set);
  //  CPPUNIT_TEST(test_add_configuration_set);
  //  CPPUNIT_TEST(test_remove_configuration_set);
  //  CPPUNIT_TEST(test_activate_configuration_set);
  //  CPPUNIT_TEST(test_getDeviceProfile);
  //  CPPUNIT_TEST(test_getServiceProfiles);
  //  CPPUNIT_TEST(test_getServiceProfile);
  //  CPPUNIT_TEST(test_getOrganizations);
  CPPUNIT_TEST_SUITE_END();

private:
  Configuration_impl* m_pConf;
  CORBA::ORB_var          m_orb;
  PortableServer::POA_var m_poa;

public:
  
  /*
   * コンストラクタ/デストラクタ
   */
  SdoConfigurationTests()
  {
    char* argv[g_argc];
    for (int i = 0; i < g_argc; i++) {
      argv[i] = (char *)g_argv[i].c_str();
    }

    m_orb = CORBA::ORB_init(g_argc, argv);
    CORBA::Object_var  obj = m_orb->resolve_initial_references("RootPOA");
    m_poa = PortableServer::POA::_narrow(obj);
  }
  
  ~SdoConfigurationTests()
  {
  }
  
  /*
   * 初期化/後始末
   */
  virtual void setUp()
  {
    m_pConf = new Configuration_impl(m_orb._retn(), m_poa._retn());
  }
  
  virtual void tearDown()
  {
    delete m_pConf;
  }

  /* tests for */
  void test_set_device_profile() {

    try {
      NVList nlist;
      // DeviceProfileのpropertiesの要素が一つの場合。 OK。
      //      nlist.length(1);
      //      CORBA::Short st, retst;
      //      st = 10;
      //      nlist[0].name = "devName";
      //      nlist[0].value <<= st;
      

      // DeviceProfileのpropertiesの要素が3つの場合。 OK。
      nlist.length(3);
      CORBA::Short st, retst;
      CORBA::Long  lg, retlg;
      CORBA::Float ft, retft;

      st = 10;
      nlist[0].name = "short data";
      nlist[0].value <<= st;

      lg = 100000;
      nlist[1].name = "long data";
      nlist[1].value <<= lg;

      ft = 1234.5;
      nlist[2].name = "float data";
      nlist[2].value <<= ft;

      DeviceProfile setProf, retProf;
      setProf.device_type = "Joystick";
      setProf.manufacturer = "Aist";
      setProf.model = "hoge";
      setProf.version = "0.4.0";
      setProf.properties = nlist;
      
      CORBA::Boolean result;
      // DeviceProfileのセット
      result = m_pConf->set_device_profile(setProf);
      
      if (!result)
	cout << "Error: set_device_profile()" << endl;

      // DeviceProfileの取得
      retProf = m_pConf->getDeviceProfile();

      string setval, retval;
      setval = setProf.device_type;
      retval = retProf.device_type;
      CPPUNIT_ASSERT(retval == setval);

      setval = setProf.manufacturer;
      retval = retProf.manufacturer;
      CPPUNIT_ASSERT(retval == setval);
      //      CPPUNIT_ASSERT(retProf.manufacturer == setProf.manufacturer);

      setval = setProf.model;
      retval = retProf.model;
      CPPUNIT_ASSERT(retval == setval);

      setval = setProf.version;
      retval = retProf.version;
      CPPUNIT_ASSERT(retval == setval);

      setval = nlist[0].name;
      retval = retProf.properties[0].name;
      CPPUNIT_ASSERT(retval == setval);
      (retProf.properties[0].value) >>= retst;
      CPPUNIT_ASSERT(retst == st);

      setval = nlist[1].name;
      retval = retProf.properties[1].name;
      CPPUNIT_ASSERT(retval == setval);
      (retProf.properties[1].value) >>= retlg;
      CPPUNIT_ASSERT(retlg == lg);

      setval = nlist[2].name;
      retval = retProf.properties[2].name;
      CPPUNIT_ASSERT(retval == setval);
      (retProf.properties[2].value) >>= retft;
      CPPUNIT_ASSERT(retft == ft);
    }
    catch (InvalidParameter ip) {
      cout << "InvalidParameter exception." << endl;
    }
    catch (NotAvailable na) {
      cout << "NotAvailable exception." << endl;
    }
    catch (InternalError ip) {
      cout << "InternalError exception." << endl;
    }
    catch (...) {
      cout << "othrer exception." << endl;
    }
  }


  /* tests for */
  void test_set_service_profile() {
    ServiceProfileList setProf, getProf;
    NameValue nv;
    NVList nvlist;
    //    SDOService service;
    CORBA::Short setst,getst;
    CORBA::Long  setlg,getlg;

    string setstr, getstr;

    setProf.length(2);
    getProf.length(2);

    try {
      setst = 10;
      nv.name = "short";
      nv.value <<= setst;
      nvlist.length(1);
      nvlist[0] = nv;

      setProf[0].id = "setProfId0";
      setProf[0].interface_type = "ifTYpe0";
      setProf[0].properties = nvlist;
      //    setProf.service = service;
      m_pConf->set_service_profile(setProf[0]);


      setlg = 1000;
      nv.name = "long";
      nv.value <<= setlg;
      nvlist.length(1);
      nvlist[0] = nv;

      setProf[1].id = "setProfId0";
      setProf[1].interface_type = "ifTYpe1";
      setProf[1].properties = nvlist;
      m_pConf->set_service_profile(setProf[1]);



      getProf[0] = m_pConf->getServiceProfile(setProf[0].id);

      setstr = setProf[0].id;
      getstr = getProf[0].id;
      CPPUNIT_ASSERT(setstr == getstr);

      setstr = setProf[0].interface_type;
      getstr = getProf[0].interface_type;
      CPPUNIT_ASSERT(setstr == getstr);

      setstr = setProf[0].properties[0].name;
      getstr = getProf[0].properties[0].name;
      CPPUNIT_ASSERT(setstr == getstr);

      getProf[0].properties[0].value >>= getst;
      CPPUNIT_ASSERT(setst == getst);

      // Failure case 同じIDのServiceProfileをセットした後、getServiceProfile(id)を呼び出すと
      //              先にセットしたオブジェクトが返される。
      //      getProf[1] = m_pConf->getServiceProfile(setProf[1].id);
      //      setstr = setProf[1].id;
      //      getstr = getProf[1].id;
      //      CPPUNIT_ASSERT(setstr == getstr);
      //      setstr = setProf[1].interface_type;
      //      getstr = getProf[1].interface_type;
      //      CPPUNIT_ASSERT(setstr == getstr);
      //      setstr = setProf[1].properties[0].name;
      //      getstr = getProf[1].properties[0].name;
      //      CPPUNIT_ASSERT(setstr == getstr);
      //      getProf[1].properties[0].value >>= getlg;
      //      CPPUNIT_ASSERT(setlg == getlg);


      //============  test for getServiceProfiles() ====================
      ServiceProfileList spList;
      spList = m_pConf->getServiceProfiles();

      for (unsigned int i = 0; i < spList.length(); i++) {
	setstr = setProf[i].id;
	getstr = spList[i].id;
	CPPUNIT_ASSERT(setstr == getstr);

	setstr = setProf[i].interface_type;
	getstr = spList[i].interface_type;
	CPPUNIT_ASSERT(setstr == getstr);

	setstr = setProf[i].properties[0].name;
	getstr = spList[i].properties[0].name;
	CPPUNIT_ASSERT(setstr == getstr);
      }
    
      spList[0].properties[0].value >>= getst;
      CPPUNIT_ASSERT(setst == getst);
      spList[1].properties[0].value >>= getlg;
      CPPUNIT_ASSERT(setlg == getlg);
      //=================================================================


      //============== test for remove_service_profile() ================
      m_pConf->remove_service_profile(setProf[0].id);
      //      m_pConf->remove_service_profile("setProfId0");
      spList = m_pConf->getServiceProfiles();

      setstr = setProf[1].id;
      getstr = spList[0].id;
      CPPUNIT_ASSERT(setstr == getstr);

      setstr = setProf[1].interface_type;
      getstr = spList[0].interface_type;
      CPPUNIT_ASSERT(setstr == getstr);

      setstr = setProf[1].properties[0].name;
      getstr = spList[0].properties[0].name;
      CPPUNIT_ASSERT(setstr == getstr);
      //=================================================================
    }
    catch (InvalidParameter ip) {
      cout << "InvalidParameter exception." << endl;
    }
    catch (NotAvailable na) {
      cout << "NotAvailable exception." << endl;
    }
    catch (InternalError ip) {
      cout << "InternalError exception." << endl;
    }
    catch (...) {
      cout << "othrer exception." << endl;
    }
  }


  /* tests for */
  void test_add_organization() {
    Organization_var orgPtr;
    CORBA::Boolean result;
    OrganizationList orgList;
    OrganizationProperty set_prop;
    //    CORBA::Short st;
    //    NVList nvList;
    //    NameValue nv;

    try {
      result = m_pConf->add_organization(orgPtr._retn());
      if (!result)
	cout << "Couldn't add organization object." << endl;

      orgList = m_pConf->getOrganizations();
      
      //      orgList[0]->set_organization_property(set_prop);
      //      orgPtr->set_organization_property(set_prop);
      //      cout << orgList[0]->get_organization_id() << endl;
    }
    catch (InvalidParameter ip) {
      cout << "InvalidParameter exception." << endl;
    }
    catch (NotAvailable na) {
      cout << "NotAvailable exception." << endl;
    }
    catch (InternalError ip) {
      cout << "InternalError exception." << endl;
    }
    catch (...) {
      cout << "othrer exception." << endl;
    }
  }


  /* tests for */
  void test_remove_service_profile() {
    //    test_set_service_profile()にてテスト
  }


  /* tests for */
  void test_remove_organization() {
    //    test_add_organization()にてテスト
  }


  /* tests for */
  void test_get_configuration_parameters() {
    ParameterList* paramList;
    
    try {

      // length 0のリストが戻される。 OK.
      paramList = m_pConf->get_configuration_parameters();
      int length = paramList->length();
      CPPUNIT_ASSERT(length == 0);

    }
    catch (InvalidParameter ip) {
      cout << "InvalidParameter exception." << endl;
    }
    catch (NotAvailable na) {
      cout << "NotAvailable exception." << endl;
    }
    catch (InternalError ip) {
      cout << "InternalError exception." << endl;
    }
    catch (...) {
      cout << "othrer exception." << endl;
    }
  }
  
  
  /* tests for */
  void test_get_configuration_parameter_values() {
    //    test_get_configuration_parameters()にてテスト
  }
  

  /* tests for */
  void test_get_configuration_parameter_value() {
    //    test_get_configuration_sets()にてテスト
  }
  
  /* tests for */
  void test_set_configuration_parameter() {
    //    test_get_configuration_sets()にてテスト
  }
  
  
  /* tests for */
  void test_get_configuration_sets() {
    ConfigurationSet confset;
    NVList nvlist;
    NameValue nv;
    CORBA::Short st, rst;
    CORBA::Long lg, rlg;
    CORBA::Float ft, rft;
    CORBA::Boolean result;
    string setname, getname;
    CORBA::Any any;
    int llength;

    try {

      //============ ConfigurationSet1 =================================
      confset.id = "configset_id1";
      confset.description = "configset_description";
      nvlist.length(1);
      nv.name = "short";
      st = 10000;
      nv.value <<= st;
      nvlist[0] = nv;
      confset.configuration_data = nvlist;
      //================================================================
      

      // add ConfigurationSet1.
      result = m_pConf->add_configuration_set(confset);
      // Activate ConfigurationSet1.
      result = m_pConf->activate_configuration_set(confset.id);
      if(!result)
	cout << "Error: activate_configuration_set()" << endl;


      //================== test for set_configuration_parameter() ===================
      st = 9;
      any <<= st;
      const char* name = "short";
      result = m_pConf->set_configuration_parameter(name, any);
      
      if(!result)
	cout << "Error: set_configuration_parameter()." << endl;
      //=============================================================================
      

      //================ test for get_configuration_parameter_values() ==============
      NVList* getList;

      setname = "short";
      getList = m_pConf->get_configuration_parameter_values();
      getname = (*getList)[0].name;
      CPPUNIT_ASSERT(getname == setname);
      (*getList)[0].value >>= rst;
      CPPUNIT_ASSERT(rst == st);
      //=============================================================================


      //============ ConfigurationSet2 =================================
      confset.id = "configset_id2";
      confset.description = "configset_description2";
      nvlist.length(1);
      nv.name = "long";
      lg = 20000;
      nv.value <<= lg;
      nvlist[0] = nv;
      confset.configuration_data = nvlist;
      //================================================================
      

      // add ConfigurationSet2.
      result = m_pConf->add_configuration_set(confset);
      // Activate ConfigurationSet2.
      result = m_pConf->activate_configuration_set(confset.id);
      if(!result)
	cout << "Error: activate_configuration_set()" << endl;


      //================ test for get_configuration_sets() ==============
      ConfigurationSetList* confSetList;
      confSetList = m_pConf->get_configuration_sets();
      llength = confSetList->length();
      CPPUNIT_ASSERT(llength == 2);

      setname = "configset_id1";
      getname = (*confSetList)[0].id;
      CPPUNIT_ASSERT(getname == setname);
      (*confSetList)[0].configuration_data[0].value >>= rst;
      CPPUNIT_ASSERT(rst == st);
      
      setname = "configset_id2";
      getname = (*confSetList)[1].id;
      CPPUNIT_ASSERT(getname == setname);
      (*confSetList)[1].configuration_data[0].value >>= rlg;
      CPPUNIT_ASSERT(rlg == lg);
      //=============================================================================


      //================ test for get_configuration_set() ==============
      ConfigurationSet* confSet;
      confSet = m_pConf->get_configuration_set("configset_id1");

      setname = "configset_id1";
      getname = confSet->id;
      CPPUNIT_ASSERT(getname == setname);
      confSet->configuration_data[0].value >>= rst;
      CPPUNIT_ASSERT(rst == st);

      confSet = m_pConf->get_configuration_set("configset_id2");

      setname = "configset_id2";
      getname = confSet->id;
      CPPUNIT_ASSERT(getname == setname);
      confSet->configuration_data[0].value >>= rlg;
      CPPUNIT_ASSERT(rlg == lg);
      //=============================================================================

      
      //================ test for set_configuration_set_values() ====================
      confset.id = "configset_id2";
      confset.description = "changed configset_description.";
      nvlist.length(1);
      nv.name = "float";
      ft = 999.999;
      nv.value <<= ft;
      nvlist[0] = nv;
      confset.configuration_data = nvlist;
      result = m_pConf->set_configuration_set_values("configset_id2", confset);

      // check by get_configuration_set();
      confSet = m_pConf->get_configuration_set("configset_id2");
      setname = "configset_id2";
      getname = confSet->id;
      CPPUNIT_ASSERT(getname == setname);
      confSet->configuration_data[0].value >>= rft;
      CPPUNIT_ASSERT(rft == ft);


      // check by get_configuration_sets();
      confSetList = m_pConf->get_configuration_sets();
      llength = confSetList->length();
      CPPUNIT_ASSERT(llength == 2);

      setname = "configset_id1";
      getname = (*confSetList)[0].id;
      CPPUNIT_ASSERT(getname == setname);
      (*confSetList)[0].configuration_data[0].value >>= rst;
      CPPUNIT_ASSERT(rst == st);
      
      setname = "configset_id2";
      getname = (*confSetList)[1].id;
      CPPUNIT_ASSERT(getname == setname);
      (*confSetList)[1].configuration_data[0].value >>= rft;
      CPPUNIT_ASSERT(rft == ft);
      //=============================================================================


      //================ test for get_active_configuration_set() ====================
      confSet = m_pConf->get_active_configuration_set();
      setname = "configset_id2";
      getname = confSet->id;
      CPPUNIT_ASSERT(getname == setname);

      string setdesc,getdesc;
      setdesc = "changed configset_description.";
      getdesc = confSet->description;
      CPPUNIT_ASSERT(getdesc == setdesc);
      //=============================================================================

      //================ test for remove_configuration_set() ========================
      result = m_pConf->remove_configuration_set("configset_id2");
      confSetList = m_pConf->get_configuration_sets();
      llength = confSetList->length();
      CPPUNIT_ASSERT(llength == 1);

      confSet = m_pConf->get_configuration_set("configset_id1");

      setname = "configset_id1";
      getname = confSet->id;
      CPPUNIT_ASSERT(getname == setname);
      confSet->configuration_data[0].value >>= rst;
      CPPUNIT_ASSERT(rst == st);

      // Failure case.
      // activateした"configset_id2"はremoveしたが、get_active_configuration_set()で取得できた。
      confSet = m_pConf->get_active_configuration_set();
      cout << confSet->id << endl;

      // InvalidParameter exception発生。 OK.
      //      confSet = m_pConf->get_configuration_set("configset_id2");
      //=============================================================================

    }
    catch (InvalidParameter ip) {
      cout << "InvalidParameter exception." << endl;
    }
    catch (NotAvailable na) {
      cout << "NotAvailable exception." << endl;
    }
    catch (InternalError ip) {
      cout << "InternalError exception." << endl;
    }
    catch (...) {
      cout << "othrer exception." << endl;
    }
  }
  

  /* tests for */
  void test_get_configuration_set() {
    //    test_get_configuration_sets()にてテスト
  }
  
  /* tests for */
  void test_set_configuration_set_values() {
    //    test_get_configuration_sets()にてテスト
  }
  
  
  /* tests for */
  void test_get_active_configuration_set() {
    //    test_get_configuration_sets()にてテスト
  }
  
  
  /* tests for */
  void test_add_configuration_set() {
    //    test_get_configuration_sets()にてテスト。
  }
  
  
  /* tests for */
  void test_remove_configuration_set() {
    //    test_get_configuration_sets()にてテスト。
  }


  /* tests for */
  void test_activate_configuration_set() {
    //    test_get_configuration_sets()にてテスト。
  }


  /* tests for */
  void test_getDeviceProfile() {
    //    test_set_device_profile()にてテスト
  }


  /* tests for */
  void test_getServiceProfiles() {
    //    test_set_service_profile()にてテスト
  }

  /* tests for */
  void test_getServiceProfile() {
    //    test_set_service_profile()にてテスト
  }

  /* tests for */
  void test_getOrganizations() {
    //    test_add_organization()にてテスト
  }
  
};


/*
 * register test suite
 */
CPPUNIT_TEST_SUITE_REGISTRATION(SdoConfigurationTests);



int main(int argc, char* argv[])
{
 
  g_argc = argc;
  for (int i = 0; i < argc; i++) {
    g_argv.push_back(argv[i]);
  }

  CppUnit::TextUi::TestRunner runner;
  
  runner.addTest(CppUnit::TestFactoryRegistry::getRegistry().makeTest());
  CppUnit::Outputter* outputter = 
    new CppUnit::TextOutputter(&runner.result(), std::cout);
  runner.setOutputter(outputter);
  
  return runner.run();
};
