#include "ParticleEmitter.h"
#include "extern.h"
#include "Parsers.h"

//HOW DOES TRANSFORM FEEDBACK WORK?
//A transform feedback is basically a mechanism by which OpenGL can tell the 
//output from a shader to be written back to a vertex buffer, (which is 
//probably part of a VAO.
//The transformfeedback object itself is just handle which points the shader attribute
//to the buffer id
//Procedure
//---------
//Create two VAOs 'A' and 'B'
//Create two empty transform feedbacks 'A' and 'B'
//For each VAO:
//create vertex array buffer with initial geometry
//use glbindbufferbase to bind the transform feedback id to the
// - buffer of bound VAO
// - out attribute id in shader
//e.g Transform Feedback A is bound to vertex buffer in VAO A
//
//Now when we draw:
//Bind VAO 'A' but bind Transform feedback *B*
//That way when we draw buffer of A, the out variable in vertex shader will save data to B
//then next frame bind VAO 'B' and transform feedback A 
//etc.


void ParticleEmitter::init() {

	const GLchar* feedback_varyings[]{
		"v_vertex",
		"v_velocity",
		"v_age",
		"v_life"
	};

	particle_shader_ = new Shader(	"data/shaders/particles.vert",
									"data/shaders/particles.frag",
									4,
									feedback_varyings);

	particle_shader_->color = lm::vec3{ 1.0f, 0.0f, 0.0f };

	texture_id_ = Parsers::parseTexture("data/assets/droptexture.tga");

	glEnable(GL_POINT_SPRITE);

	//tell opengl that the shader set the point site
	glEnable(GL_PROGRAM_POINT_SIZE);
	
	int num_particles = 1000;
	std::vector<GLfloat> vertices(num_particles * 3, 0);
	std::vector<GLfloat> velocities(num_particles * 3, 0);
	std::vector<GLfloat> ages(num_particles, 0);
	std::vector<GLfloat> lives(num_particles, 0);

	for (int i = 0; i < num_particles; i++){
		lives[i] = (float)(rand() % 9000) / 9000.0f;
	}

	//create vaos and transform feedbacks
	glGenVertexArrays(1, &vaoA_);
	glGenVertexArrays(1, &vaoB_);

	glGenTransformFeedbacks(1, &tfA_);
	glGenTransformFeedbacks(1, &tfB_);

	glBindVertexArray(vaoA_);
	GLuint vb_A_pos, vb_A_vel, vb_A_age, vb_A_lif;

	//positions buffer
	glGenBuffers(1, &vb_A_pos);
	glBindBuffer(GL_ARRAY_BUFFER, vb_A_pos);
	//fill buffer with this data
	glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(GLfloat), &(vertices[0]), GL_STREAM_COPY);

	//link to variable in shader
	glEnableVertexAttribArray(0); //kayout(location = 0) in
	glVertexAttribPointer(	0, // must be same as above!
							3, //x,y,z
							GL_FLOAT,
							GL_FALSE,
							0,
							0); 

	//velocities buffer
	glGenBuffers(1, &vb_A_vel);
	glBindBuffer(GL_ARRAY_BUFFER, vb_A_vel);
	//fill buffer with this data
	glBufferData(GL_ARRAY_BUFFER, velocities.size() * sizeof(GLfloat), &(velocities[0]), GL_STREAM_COPY);

	//link to variable in shader
	glEnableVertexAttribArray(1); 
	glVertexAttribPointer(1, 
		3, //x,y,z
		GL_FLOAT,
		GL_FALSE,
		0,
		0);


	//ages buffer
	glGenBuffers(1, &vb_A_age);
	glBindBuffer(GL_ARRAY_BUFFER, vb_A_age);
	//fill buffer with this data
	glBufferData(GL_ARRAY_BUFFER, ages.size() * sizeof(GLfloat), &(ages[0]), GL_STREAM_COPY);

	//link to variable in shader
	glEnableVertexAttribArray(2);
	glVertexAttribPointer(2,
		1, 
		GL_FLOAT,
		GL_FALSE,
		0,
		0);


	//life buffer
	glGenBuffers(1, &vb_A_lif);
	glBindBuffer(GL_ARRAY_BUFFER, vb_A_lif);
	//fill buffer with this data
	glBufferData(GL_ARRAY_BUFFER, lives.size() * sizeof(GLfloat), &(lives[0]), GL_STREAM_COPY);

	//link to variable in shader
	glEnableVertexAttribArray(3);
	glVertexAttribPointer(3,
		1, 
		GL_FLOAT,
		GL_FALSE,
		0,
		0);


	glBindTransformFeedback(GL_TRANSFORM_FEEDBACK, tfA_);
	glBindBufferBase(GL_TRANSFORM_FEEDBACK_BUFFER, 0, vb_A_pos);
	glBindBufferBase(GL_TRANSFORM_FEEDBACK_BUFFER, 1, vb_A_vel);
	glBindBufferBase(GL_TRANSFORM_FEEDBACK_BUFFER, 2, vb_A_age);
	glBindBufferBase(GL_TRANSFORM_FEEDBACK_BUFFER, 3, vb_A_lif);


	//to here
	glBindVertexArray(vaoB_);
	GLuint vb_B_pos, vb_B_vel, vb_B_age, vb_B_lif;

	//positions buffer
	glGenBuffers(1, &vb_B_pos);
	glBindBuffer(GL_ARRAY_BUFFER, vb_B_pos);
	//fill buffer with this data
	glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(GLfloat), &(vertices[0]), GL_STREAM_COPY);

	//link to variable in shader
	glEnableVertexAttribArray(0); //kayout(location = 0) in
	glVertexAttribPointer(0, // must be same as above!
		3, //x,y,z
		GL_FLOAT,
		GL_FALSE,
		0,
		0);

	//velocities buffer
	glGenBuffers(1, &vb_B_vel);
	glBindBuffer(GL_ARRAY_BUFFER, vb_B_vel);
	//fill buffer with this data
	glBufferData(GL_ARRAY_BUFFER, velocities.size() * sizeof(GLfloat), &(velocities[0]), GL_STREAM_COPY);

	//link to variable in shader
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1,
		3, //x,y,z
		GL_FLOAT,
		GL_FALSE,
		0,
		0);


	//ages buffer
	glGenBuffers(1, &vb_B_age);
	glBindBuffer(GL_ARRAY_BUFFER, vb_B_age);
	//fill buffer with this data
	glBufferData(GL_ARRAY_BUFFER, ages.size() * sizeof(GLfloat), &(ages[0]), GL_STREAM_COPY);

	//link to variable in shader
	glEnableVertexAttribArray(2);
	glVertexAttribPointer(2,
		1,
		GL_FLOAT,
		GL_FALSE,
		0,
		0);


	//life buffer
	glGenBuffers(1, &vb_B_lif);
	glBindBuffer(GL_ARRAY_BUFFER, vb_B_lif);
	//fill buffer with this data
	glBufferData(GL_ARRAY_BUFFER, lives.size() * sizeof(GLfloat), &(lives[0]), GL_STREAM_COPY);

	//link to variable in shader
	glEnableVertexAttribArray(3);
	glVertexAttribPointer(3,
		1,
		GL_FLOAT,
		GL_FALSE,
		0,
		0);


	glBindTransformFeedback(GL_TRANSFORM_FEEDBACK, tfB_);
	glBindBufferBase(GL_TRANSFORM_FEEDBACK_BUFFER, 0, vb_B_pos);
	glBindBufferBase(GL_TRANSFORM_FEEDBACK_BUFFER, 1, vb_B_vel);
	glBindBufferBase(GL_TRANSFORM_FEEDBACK_BUFFER, 2, vb_B_age);
	glBindBufferBase(GL_TRANSFORM_FEEDBACK_BUFFER, 3, vb_B_lif);

}

void ParticleEmitter::update() {
	glUseProgram(particle_shader_->program);

	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glDepthMask(GL_FALSE);
	
	Camera& cam = ECS.getComponentInArray<Camera>(ECS.main_camera);

	particle_shader_->setUniform(U_MODEL, lm::mat4());
	particle_shader_->setUniform(U_VP, cam.view_projection);
	particle_shader_->setTexture(U_DIFFUSE_MAP, texture_id_, 0);
	particle_shader_->setUniform(U_COLOR, lm::vec3(color[0], color[1], color[2]));
	//time in seconds since app starts
	particle_shader_->setUniform(U_TIME, (float)glfwGetTime());

	int viewport[4];
	glGetIntegerv(GL_VIEWPORT, viewport);
	float height_near_plane = std::abs(viewport[3] - viewport[1]) / (2 * tan(0.5 * cam.fov));
	particle_shader_->setUniform(U_HEIGHT_NEAR_PLANE, height_near_plane);

	if (vaoSource == 0) {
		glBindVertexArray(vaoA_);
		glBindTransformFeedback(GL_TRANSFORM_FEEDBACK, tfB_);
		vaoSource = 1;
	}
	else {
		glBindVertexArray(vaoB_);
		glBindTransformFeedback(GL_TRANSFORM_FEEDBACK, tfA_);
		vaoSource = 0;
	}

	glBeginTransformFeedback(GL_POINTS);
	glDrawArrays(GL_POINTS, 0, 1000);
	glEndTransformFeedback();
	glDisable(GL_BLEND);
	glDepthMask(GL_TRUE);
}
