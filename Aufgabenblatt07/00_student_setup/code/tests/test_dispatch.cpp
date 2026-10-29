// Regression test for the compute shader dispatch (QA-007).
//
// The separable Gaussian dispatches one invocation per pixel with a 4x4 work
// group. Truncating width / 4 and height / 4 skipped the border of images
// whose size is not a multiple of four.
#include "DispatchUtils.h"

#include <iostream>

namespace
{
    int failures = 0;

    void check(const unsigned int size, const unsigned int group_size, const unsigned int expected)
    {
        const unsigned int actual = cg::compute_group_count(size, group_size);

        if (actual != expected)
        {
            std::cerr << "FAIL: " << size << " / " << group_size << " -> " << actual << " (expected " << expected
                      << ")\n";
            ++failures;
        }
    }
}

int main()
{
    check(4, 4, 1);
    check(5, 4, 2);
    check(502, 4, 126);
    check(1919, 4, 480);
    check(1920, 4, 480);
    check(0, 4, 0);

    if (failures != 0)
    {
        std::cerr << failures << " check(s) failed\n";
        return 1;
    }

    std::cout << "All compute dispatch checks passed\n";
    return 0;
}
