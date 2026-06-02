@group(0) @binding(0) var in_texture: texture_2d<f32>;
@group(1) @binding(0) var<uniform> u_time: f32;

struct VertexInput {
    @builtin(instance_index) instance_id : u32,
    @location(0) position: vec3f,
#unique vertex @location(1) uv: vec2f,
#unique vertex @location(2) normal: vec3f,
#unique vertex @location(3) tangent: vec4f,
#unique vertex @location(4) color: vec3f,
#unique vertex @location(5) weights: vec4f,
#unique vertex @location(6) joints: vec4i
};

struct DefferedVertexOut {
    @builtin(position)  position: vec4f,
    @location(0) uv: vec2f,
};

@vertex
fn vs_main(in: VertexInput) -> DefferedVertexOut {
    var out: DefferedVertexOut;
    out.position = vec4f(in.position, 1.0);
    out.uv = in.uv;
    return out;
}

struct FragmentOutput {
    @location(0) color: vec4f
}

@fragment
fn fs_main(in: DefferedVertexOut, @builtin(front_facing) is_front_facing: bool) -> FragmentOutput {
    var out: FragmentOutput;
    let screen_dims = textureDimensions(in_texture);
    let pixel = vec2<i32>(in.uv * vec2<f32>(screen_dims));
    
    var distance = abs(distance(in.uv,vec2f(0.5)));
    distance = distance*distance*1.5;
    //if blending fails
    let time = u_time / 1.0;
    let color = textureLoad(in_texture, pixel, 0);
    let mixed_color = mix(color,vec4f(1.0,0.0,0.0,1.0),distance*time);
    out.color = vec4f(mixed_color.rgb,1.0);
    

   // out.color = vec4f(vec3f(1.0,0.0,0.0),distance*u_time);

    return out;
}