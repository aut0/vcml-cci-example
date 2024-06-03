#include <systemc>
#include <cci_configuration>

#include <vcml.h>
#include <vcml-cci.h>

class vcml_timer : public vcml::module {
public:
    vcml::property<int> p_tval;

    vcml_timer(const sc_core::sc_module_name& nm)
            : vcml::module(nm)
            , p_tval("timer_value", 10)
            , m_tval(p_tval) {
        SC_THREAD(timer_loop);
    }

private:
    unsigned int m_tval;

    void timer_loop() {
        while (m_tval > 0) {
            std::cout << name() << " current timer value: " << m_tval << std::endl;
            m_tval--;
            sc_core::wait(1, sc_core::SC_NS);
        }
    }
};

class simple_timer : public sc_core::sc_module {
public:
    cci::cci_param<int> p_tval;

    simple_timer(const sc_core::sc_module_name& nm)
            : sc_core::sc_module(nm)
            , p_tval("timer_value", 10, "timer start value")
            , m_tval(p_tval.get_value()) {
        SC_THREAD(timer_loop);
    }

private:
    unsigned int m_tval;

    void timer_loop() {
        while (m_tval > 0) {
            std::cout << name() << " current timer value: " << m_tval << std::endl;
            m_tval--;
            sc_core::wait(1, sc_core::SC_NS);
        }
    }
};

int sc_main(int argc, char* argv[]) {
    cci_utils::broker global_broker("CCI Global Broker");
    cci::cci_register_broker(global_broker);

    cci::cci_value my_value(cci::cci_value::from_json("123"));
    cci::cci_originator my_orig("sc_main");
    global_broker.set_preset_cci_value("simple_timer.timer_value", my_value, my_orig);

    vcml::cci::broker vcml_cci_broker("vcml_cci_broker");
    global_broker.set_preset_cci_value("vcml_timer.timer_value", my_value, my_orig);

    simple_timer foo("simple_timer");
    vcml_timer bar("vcml_timer");

    sc_start(1, sc_core::SC_MS);
    return 0;
}
