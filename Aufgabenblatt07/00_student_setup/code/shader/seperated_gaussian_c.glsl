#version 430

/** Input image texture */
layout(rgba32f) readonly uniform image2D src_tx2D;
/** Output image texture */
layout(rgba32f) writeonly uniform image2D tgt_tx2D;

/** Texture with kernel values */
layout(r32f) readonly uniform image2D kernel_tx2D;

/** Image size */
uniform ivec2 img_size;
/** Extent of filter kernel */
uniform int kernel_extents;
/**
 * Offset direction.
 * For horizontal filter pass it is set to vec2(1,0).
 * For the vertical filter pass it is set to vec2(0,1).
 */
uniform ivec2 pixel_offset;

layout(local_size_x = 4, local_size_y = 4, local_size_z = 1) in;

void main()
{
    ivec3 global_pos = ivec3(gl_GlobalInvocationID.xyz);

    // early exit if global id outside of grid (due to group size)
    if( global_pos.x >= img_size.x || global_pos.y >= img_size.y )
        return;

    // Intermediate storage for compute sum of multiplications
    vec4 pixel_value = vec4(0.0);

	for(int i = -kernel_extents; i <= kernel_extents; i++)
	{
        ivec2 kernel_coords = ivec2(kernel_extents+i,0.5);
        ivec2 img_coords = global_pos.xy + i * pixel_offset;

        float k = imageLoad(kernel_tx2D,kernel_coords).x;

		pixel_value += imageLoad(src_tx2D, img_coords) * k;
	}

    imageStore(tgt_tx2D, global_pos.xy, pixel_value);
}
