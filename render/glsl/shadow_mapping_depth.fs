#version 330 core

void main()
{             
    // gl_FragDepth = gl_FragCoord.z;
    gl_FragColor = vec4(gl_FragCoord.z,1.0f,1.0f,1.0f);
}