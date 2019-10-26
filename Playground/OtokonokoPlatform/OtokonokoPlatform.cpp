#include <covscript/covscript.hpp>

#include "OtokonokoPlatformRuntime.h"

#include <iostream>

//// 两种方式，一种是自己做转发，另外一种就是声明转发规则
//// 不过这个函数因为有一些没用的参数，就必须转发了，这里仅示范自动转发规则的编写
//
//// 全局自动转换规则
//namespace cs_impl
//{
//
//    // CovScript->C++转换
//    // 可以多个C++类型对应一个CovScript类型
//    template<>
//    struct type_conversion_cs<const char*>
//    {
//        using source_type = cs::string;
//    };
//
//    // C++->CovScript转换
//    // 可以多个C++类型对应一个CovScript类型
//    template<>
//    struct type_conversion_cpp<const char*>
//    {
//        using target_type = cs::string;
//    };
//
//    // 自定义转换函数
//    // 一般能够static_cast的转换CovScript能够自动检测到
//    // 这里对convert函数的要求不统一，需要的时候可以上Universal Reference
//    template<>
//    struct type_convertor<cs::string, const char*>
//    {
//        static const char* convert(const cs::string& val)
//        {
//            return val.c_str();
//        }
//    };
//}
//
//
//
//void message_box(const char* title, const char* text)
//{
//    MessageBoxA(nullptr, text, title, MB_ICONINFORMATION);
//}
//
//void Entry()
//{
//    cs::namespace_t context_ext = cs::make_shared_namespace<cs::name_space>();
//
//    cs::context_t cxt = cs::create_context({});
//
//    // 设置名称空间
//    cs::namespace_t dismpp_ns = cs::make_shared_namespace<cs::name_space>();
//
//    /*
//    1. 遵循全局自动转发规则+CovScript自动检测转发规则
//    2. 自定义转发规则
//    3. 同（1），但是通过Lambda表达式自己做了转发，一般推荐这样做
//    */
//    (*dismpp_ns)
//        .add_var("message_box_1", cs::make_cni(message_box))
//        .add_var("message_box_2", cs::make_cni(message_box, cs::cni_type<void(cs::string, cs::string)>()))
//        .add_var("message_box_3", cs::make_cni([](const cs::string& title, const cs::string& text) {
//        MessageBoxA(nullptr, text.c_str(), title.c_str(), MB_ICONINFORMATION);
//            }));
//
//    // 编译前将名称空间加入存储器
//    cxt->instance->storage.add_buildin_var("dismpp", dismpp_ns);
//    cxt->instance->compile("D:\\hello_world_cleanup.csc");
//    cxt->instance->interpret();
//
//    cs::function_invoker<void()> cs_main(cs::eval(cxt, "main"));
//
//    cs_main();
//}

int main()
{
    std::wprintf(
        L"Otokonoko no Sekai Version 0.1 [Build 1]\r\n"
        L"Copyright (C) 2019 Covariant Institute. All rights reserved.\r\n"
        L"\r\n"
        L"\r\n"
        L"Otokonoko Platform\r\n"
        L"\r\n"
        L"\r\n");

    int errorcode = 0;
    try
    {
        cs::context_t context = cs::create_context({});
        cs::current_process->on_process_exit.add_listener([&context](void* code) -> bool
            {
            cs::current_process->exit_code = *static_cast<int*>(code);
            throw cs::fatal_error("CS_REPL_EXIT");
            return true;
            });
        cs::repl repl(context);
        std::string line;
        while (std::cin)
        {
            std::cout << std::string(repl.get_level() * 2, '.') << "> " << std::flush;
            std::getline(std::cin, line);
            try
            {
                repl.exec(line);
            }
            catch (const std::exception & e)
            {
                if (std::strstr(e.what(), "CS_REPL_EXIT") == nullptr)
                {
                    WriteLine(e.what());
                }
                else
                    break;
            }
            catch (...) {
                cs::collect_garbage(context);
                throw;
            }
        }
        cs::collect_garbage(context);

        errorcode = cs::current_process->exit_code;
    }
    catch (const std::exception & e)
    {
        WriteLine(e.what());
        errorcode = -1;
    }
    catch (...)
    {
        WriteLine("Uncaught exception: Unknown exception");
        errorcode = -1;
    }

    return 0;
}
