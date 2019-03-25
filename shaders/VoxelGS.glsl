#version 420
 
layout(points) in;
layout (triangle_strip, max_vertices=24) out;

uniform float inVoxelSize = 1;

uniform mat4 inVertToWorld = mat4(1);
uniform mat4 inWorldToCamera = mat4(1);
uniform mat4 inCameraToClip = mat4(1);
uniform mat4 inShadowMapProjection;
uniform int inUseShadowMap = 0;

flat in int colourIdx[];

centroid out vec4 shadowMapPos;
flat out int colourIdxFS;
flat out vec3 normalFS;

void submitVertex(float x, double y, double z, vec3 normal)
  {
  normalFS = normal;
  vec3 vert = vec3(x, y, z) + vec3(gl_in[0].gl_Position);
  vert *= inVoxelSize;
  vert += inVoxelSize * 0.5;

  vec4 worldVertex = vec4(vert, 1) * inVertToWorld;
  gl_Position = worldVertex * inWorldToCamera * inCameraToClip;

  if (inUseShadowMap >= 1)
    shadowMapPos = worldVertex * inShadowMapProjection;

  colourIdxFS = colourIdx[0];
  EmitVertex();
  }

vec3 rotatedNormal(float x, float y, float z)
  {
  return normalize(vec3(x, y, z) * mat3(inVertToWorld));
  }

void main()
  {
  vec3 normal;

  normal = rotatedNormal(0, -1, 0);
  submitVertex(0.5,     -0.5,    -0.5, normal);
  submitVertex(-0.5,    -0.5,    -0.5, normal);
  submitVertex(0.5,     -0.5,    0.5, normal);
  submitVertex(-0.5,    -0.5,    0.5, normal);
  EndPrimitive();

  normal = rotatedNormal(0, 1, 0);
  submitVertex(0.5,     0.5,    -0.5, normal);
  submitVertex(-0.5,    0.5,    -0.5, normal);
  submitVertex(0.5,     0.5,    0.5, normal);
  submitVertex(-0.5,    0.5,    0.5, normal);
  EndPrimitive();

  normal = rotatedNormal(1, 0, 0);
  submitVertex(0.5,     0.5,     -0.5, normal);
  submitVertex(0.5,     0.5,     0.5, normal);
  submitVertex(0.5,     -0.5,    -0.5, normal);
  submitVertex(0.5,     -0.5,    0.5, normal);
  EndPrimitive();


  normal = rotatedNormal(-1, 0, 0);
  submitVertex(-0.5,     0.5,     -0.5, normal);
  submitVertex(-0.5,     -0.5,    -0.5, normal);
  submitVertex(-0.5,     0.5,     0.5, normal);
  submitVertex(-0.5,     -0.5,    0.5, normal);
  EndPrimitive();


  normal = rotatedNormal(0, 0, 1);
  submitVertex(0.5,     -0.5,    0.5, normal);
  submitVertex(0.5,     0.5,     0.5, normal);
  submitVertex(-0.5,    -0.5,    0.5, normal);
  submitVertex(-0.5,    0.5,     0.5, normal);
  EndPrimitive();

  normal = rotatedNormal(0, 0, -1);
  submitVertex(0.5,     -0.5,    -0.5, normal);
  submitVertex(-0.5,    -0.5,    -0.5, normal);
  submitVertex(0.5,     0.5,     -0.5, normal);
  submitVertex(-0.5,    0.5,     -0.5, normal);
  EndPrimitive();
  }