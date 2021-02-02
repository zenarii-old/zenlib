#!/usr/bin/env python
#Abi Baxter 2020

def getShaderName(shader):
	period_index = 0;
	for ch in shader:
		if ch == ".": break;
		period_index += 1
	return shader[:period_index]

def generateShader(shader):
	name = getShaderName(shader);

	info_string = "\t{\n";
	info_string += "\t\t\"" + name + "\",\n";

	edit_mode = "none";
	vertex_source = "";
	fragment_source = "";
	lines = [];
	with open (shader) as file:
		lines = file.readlines()

	for line in lines:
		line = line.rstrip("\n");
		if line.lower() == "@vertex":
			edit_mode = "vert";
			continue;
		elif line.lower() == "@fragment":
			edit_mode = "frag";
			continue;
		elif line.lower() == "@end":
			edit_mode = "none";		
			continue;
		
		if edit_mode == "vert":
			vertex_source += "\t\t\"" + line + "\\n\" \\\n";
		elif edit_mode == "frag":
			fragment_source += "\t\t\"" + line + "\\n\" \\\n";

	vertex_source = vertex_source.rstrip("\n \\");
	fragment_source = fragment_source.rstrip("\n \\");
	info_string += vertex_source + ",\n";
	info_string += fragment_source + "\n\t},\n";

	return info_string


def main():
	shader_names = ["rgba.glsl", "lighting.glsl", "textures.glsl"];
	final_string = "";
	final_string += "zen3d_shader_info ShaderInfo[] = {\n"
	for shader in shader_names:
		final_string += generateShader(shader);
	final_string += "};"

	with open("generated_opengl_shaders.inc", "w") as file:
		file.write(final_string);

if __name__ == "__main__":
	main();