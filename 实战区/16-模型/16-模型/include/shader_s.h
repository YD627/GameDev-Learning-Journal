#ifndef SHADER_H
#define SHADER_H

#include<glad/glad.h>

#include<iostream>
#include<fstream>
#include<sstream>
#include<string>
#include<glm/glm.hpp>
#include<glm/gtc/matrix_transform.hpp>
#include<glm/gtc/type_ptr.hpp>
using namespace std;

class Shader {
public:
	unsigned int ID;
	Shader(const string vertexPath, const string fragmentPath) {
		string vetexCode;
		string fragmentCode;
		ifstream vShaderFile;
		ifstream fShaderFile;

		vShaderFile.exceptions(ifstream::failbit | ifstream::badbit);
		fShaderFile.exceptions(ifstream::failbit | ifstream::badbit);

		try {
			vShaderFile.open(vertexPath);
			fShaderFile.open(fragmentPath);
			stringstream vShaderStream, fShaderStream;
			vShaderStream << vShaderFile.rdbuf();
			fShaderStream << fShaderFile.rdbuf();

			vShaderFile.close();
			fShaderFile.close();

			vetexCode = vShaderStream.str();
			fragmentCode = fShaderStream.str();
		}
		catch (ifstream::failure& e) {
			cout << "文件读取失败" <<  endl;
		}

		const char* vShaderCode = vetexCode.c_str();
		const char* fShaderCode = fragmentCode.c_str();

		unsigned int vShader, fShader;

		vShader = glCreateShader(GL_VERTEX_SHADER);
		glShaderSource(vShader, 1, &vShaderCode, NULL);
		glCompileShader(vShader);
		checkCompilerErrors(vShader, "VERTEX");

		fShader = glCreateShader(GL_FRAGMENT_SHADER);
		glShaderSource(fShader, 1, &fShaderCode, NULL);
		glCompileShader(fShader);
		checkCompilerErrors(fShader, "FRAGMENT");

		ID = glCreateProgram();
		glAttachShader(ID, vShader);
		glAttachShader(ID, fShader);
		glLinkProgram(ID);
		checkCompilerErrors(ID, "PROGRAM");

		glDeleteShader(vShader);
		glDeleteShader(fShader);
	}
	void use() {
		glUseProgram(ID);
	}
	void setBool(const string& name, bool value) const {
		glUniform1i(glGetUniformLocation(ID, name.c_str()), (int)value);
	}
	void setInt(const string& name, int value) const {
		glUniform1i(glGetUniformLocation(ID, name.c_str()), value);
	}
	void setFloat(const string& name, float value) const {
		glUniform1f(glGetUniformLocation(ID, name.c_str()), value);
	}
	void setVec3(const string& name, const glm::vec3& value) const {
		glUniform3fv(glGetUniformLocation(ID, name.c_str()), 1, &value[0]);
	}
	void setVec3(const string& name, float x, float y, float z) const {
		glUniform3f(glGetUniformLocation(ID, name.c_str()), x, y, z);
	}
	void setMat4(const std::string& name, const glm::mat4& mat) const
	{
		glUniformMatrix4fv(glGetUniformLocation(ID, name.c_str()), 1, GL_FALSE, &mat[0][0]);
	}
private:
	void checkCompilerErrors(unsigned int shader, string type) {
		int success;
		char infoLog[1024];
		if (type != "PROGRAM") {
			glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
			if (!success) {
				glGetShaderInfoLog(shader, 1024, NULL, infoLog);
				cout << "编译错误类型: " << type << "\n" << infoLog << endl;
			}
		}
		else {
			glGetProgramiv(shader, GL_LINK_STATUS, &success);
			if (!success) {
				glGetProgramInfoLog(shader, 1024, NULL, infoLog);
				cout << "程序链接错误，错误类型: " << type << "\n" << infoLog << endl;
			}
		}
	}
};

#endif

