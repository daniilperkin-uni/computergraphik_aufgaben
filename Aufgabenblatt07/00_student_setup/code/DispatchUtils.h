#pragma once

namespace cg
{
    /// <summary>
    /// Number of compute work groups needed to cover `size` invocations with
    /// `group_size` threads per group. The result is rounded up, otherwise the
    /// last 1..group_size-1 invocations (the right/bottom image border) would
    /// never be processed.
    /// </summary>
    inline unsigned int compute_group_count(const unsigned int size, const unsigned int group_size)
    {
        return (size + group_size - 1) / group_size;
    }
}
