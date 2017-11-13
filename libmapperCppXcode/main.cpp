//
//  main.cpp
//  libmapperCppXcode
//
//

#include <iostream>
#include <mapper/mapper_cpp.h>

void handler(mapper_signal sig, mapper_id instance, const void *value,
             int count, mapper_timetag_t *timetag)
{
    // note we are using C API here since C++ headers don't handle callbacks yet
    if (value) {
        printf("--> signal %s received", mapper_signal_name(sig));
        float *v = (float*)value;
        for (int i = 0; i < mapper_signal_length(sig); i++) {
            std::cout << v[i];
        }
        std::cout << std::endl;
    }
}

int main(int argc, char ** argv)
{
    unsigned int i = 0;
    
    // create device
    mapper::Device dev("mydevice");
    
    // add input signal
    dev.add_input_signal("in1", 1, 'f', 0, 0, 0, handler);
    
    // add output signal
    mapper::Signal out = dev.add_output_signal("out1", 1, 'f', "na");
    
    // wait for device to be ready
    while (!dev.ready()) {
        dev.poll(10);
    }
    
    // print out name and num signals
    std::cout << "device " << dev.name() << " ready..." << std::endl;
    std::cout << "  num_inputs: " << dev.num_signals(MAPPER_DIR_INCOMING) << std::endl;
    std::cout << "  num_outputs: " << dev.num_signals(MAPPER_DIR_OUTGOING) << std::endl;
    
    // loop: poll device and update output signal
    while (1) {
        dev.poll(100);
        out.update((float)i++);
    }
    
    return i ? 0 : 1;
}
