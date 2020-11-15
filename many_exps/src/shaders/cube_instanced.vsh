#version 430

layout (location=0) in vec3 position;

uniform mat4 M;
uniform mat4 V;
uniform mat4 P;
uniform float tf;

out vec4 varyingColor;

mat4 buildRotateX(float rad);
mat4 buildRotateY(float rad);
mat4 buildRotateZ(float rad);
mat4 buildTranslate(float a, float b, float c);

void main( void ){	
    float i = gl_InstanceID + tf;
    float a = sin(203.0*i/8000.0)*403.0;
    float b = sin(301.0*i/4001.0)*401.0;
    float c = sin(400.0*i/6003.0)*405.0;
    // Build the required transformations
    mat4 localRotX = buildRotateX(10*i);
    mat4 localRotY = buildRotateY(10*i);
    mat4 localRotZ = buildRotateZ(10*i);
    mat4 localTrans = buildTranslate(a, b, c);
    // Model and view matrix
    mat4 nM = M*localTrans*localRotX*localRotY*localRotZ;
    mat4 MV = V*nM;
    gl_Position = P*MV*vec4(position, 1.0);
    varyingColor = vec4(position, 1.0)*0.5 + vec4(0.5, 0.5, 0.5, 0.5);
}

mat4 buildTranslate(float a, float b, float c){
    mat4 trans = mat4(1.0, 0.0, 0.0, 0.0,
                      0.0, 1.0, 0.0, 0.0,
                      0.0, 0.0, 1.0, 0.0, 
                      a, b, c, 1.0);
    return trans;
}

mat4 buildRotateX(float rad){
    mat4 rot = mat4(1.0, 0.0, 0.0, 0.0,
                    0.0, cos(rad), -sin(rad), 0.0,
                    0.0, sin(rad), cos(rad), 0.0, 
                    0.0, 0.0, 0.0, 1.0);
    return rot;
}

mat4 buildRotateY(float rad){
    mat4 rot = mat4(cos(rad), 0.0, sin(rad), 0.0,
                    0.0, 1.0, 0.0, 0.0,
                    -sin(rad), 0.0, cos(rad), 0.0, 
                    0.0, 0.0, 0.0, 1.0);
    return rot;
}

mat4 buildRotateZ(float rad){
    mat4 rot = mat4(cos(rad), -sin(rad), 0.0, 0.0,
                    sin(rad), cos(rad), 0.0, 0.0,
                    0.0, 0.0, 1.0, 0.0, 
                    0.0, 0.0, 0.0, 1.0);
    return rot;
}
