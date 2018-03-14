#ifndef _INI_PARSER_H_
#define _INI_PARSER_H_
#include <string>
#include <boost/property_tree/ptree.hpp>    
#include <boost/property_tree/ini_parser.hpp>

using std::string;

/**
 * 基于boost库的INI配置文件的读写操作类 
 */
class IniConfig
{
public:
	/**
 	* 构造函数
 	* 参数: @filename: 配置文件的全路径
 	*/	 
	IniConfig(const string &filename):m_ini_filename(filename)
	{
		boost::property_tree::ini_parser::read_ini(filename,m_ptree);
	}

	/**
 	* 析构函数
 	*/
	~IniConfig()
	{
		m_ptree.clear();
	}

	/**
 	* 根据指定的section和Key读取string类型的配置值
 	* 参数: 
 	*  @section , 节名称
 	*  @key ,   键名称
 	*  @default_value , 当不存在或失败时返回的默认值
 	* 返回值 : 配置文件中的对应string值
 	*/
	string ReadProfileString(const string &section,const string &key,const string &default_value="")
	{
		boost::property_tree::ptree section_setting=m_ptree.get_child(section);
		return section_setting.get<string>(key,default_value);
	}

        /**
         * 根据指定的section和Key读取interger类型的配置值
         * 参数: 
         *  @section , 节名称
         *  @key , 键名称
         *  @default_value , 当不存在或失败时返回的默认值
         * 返回值 : 配置文件中的对应interger值
         */	
	int ReadProfileInt(const string &section,const string &key,const int &default_value=0)
	{
        	boost::property_tree::ptree section_setting=m_ptree.get_child(section);
        	return section_setting.get<int>(key,default_value);
	}

        /**
        * 根据指定的section和Key写入string类型的配置值
        * 参数: 
        *  @section , 节名称
        *  @key ,   键名称
        *  @value , 值
        */

	void WriteProfileString(const string &section,const string &key,const string &value)
	{
		boost::property_tree::ptree &section_setting=m_ptree.get_child(section);
		section_setting.put<string>(key,value);
		boost::property_tree::ini_parser::write_ini(m_ini_filename,m_ptree);
	}

        /**
        * 根据指定的section和Key写入integer类型的配置值
        * 参数: 
        *  @section , 节名称
        *  @key ,   键名称
        *  @value , 值
        */
	void WriteProfileInt(const string &section,const string &key,const int &value)
	{
		boost::property_tree::ptree &section_setting=m_ptree.get_child(section);
		section_setting.put<int>(key,value);
		boost::property_tree::ini_parser::write_ini(m_ini_filename,m_ptree);
	}

private:
	string m_ini_filename;
	boost::property_tree::ptree m_ptree;
};

/*
IniConfig ini("./config.conf");
std::cout<<"host:"<<ini.ReadProfileString("mysql","host","127.0.0.1")<<std::endl;
std::cout<<"port:"<<ini.ReadProfileInt("mysql","port",3306)<<std::endl;
ini.WriteProfileString("mysql","host","10.1.1.1");
ini.WriteProfileInt("mysql","port",3307);
*/
#endif
