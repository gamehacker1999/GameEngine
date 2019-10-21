#include "Skeleton.h"
#include<string>
#include<sstream>
#include<fstream>
#include<rapidjson/document.h>
#include<SDL.h>

Skeleton::Skeleton()
{
}


Skeleton::~Skeleton()
{
}

bool Skeleton::Load(const std::string & fileName)
{
	std::ifstream file(fileName);
	if (!file.is_open())
	{
		SDL_Log("File not found: Skeleton %s", fileName.c_str());
		return false;
	}

	std::stringstream fileStream;
	fileStream << file.rdbuf();
	std::string contents = fileStream.str();
	rapidjson::StringStream jsonStr(contents.c_str());
	rapidjson::Document doc;
	doc.ParseStream(jsonStr);

	if (!doc.IsObject())
	{
		SDL_Log("Skeleton %s is not valid json", fileName.c_str());
		return false;
	}

	int ver = doc["version"].GetInt();

	// Check the metadata
	if (ver != 1)
	{
		SDL_Log("Skeleton %s unknown format", fileName.c_str());
		return false;
	}

	const rapidjson::Value& bonecount = doc["bonecount"];
	if (!bonecount.IsUint())
	{
		SDL_Log("Skeleton %s doesn't have a bone count.", fileName.c_str());
		return false;
	}

	size_t count = bonecount.GetUint();

	if (count > 255)
	{
		SDL_Log("Skeleton %s exceeds maximum bone count.", fileName.c_str());
		return false;
	}

	bones.reserve(count);

	const rapidjson::Value& bones = doc["bones"];
	if (!bones.IsArray())
	{
		SDL_Log("Skeleton %s doesn't have a bone array?", fileName.c_str());
		return false;
	}

	if (bones.Size() != count)
	{
		SDL_Log("Skeleton %s has a mismatch between the bone count and number of bones", fileName.c_str());
		return false;
	}

	Bone temp;

	//for each bone in skeleton	
	for (rapidjson::SizeType i = 0; i < count; i++)
	{
		if (!bones[i].IsObject())
		{
			SDL_Log("Skeleton %s: Bone %d is invalid.", fileName.c_str(), i);
			return false;
		}

		const rapidjson::Value& name = bones[i]["name"];
		temp.name = name.GetString();

		const rapidjson::Value& parent = bones[i]["parent"];
		temp.parentIndex = parent.GetInt();

		const rapidjson::Value& bindpose = bones[i]["bindpose"];
		if (!bindpose.IsObject())
		{
			SDL_Log("Skeleton %s: Bone %d is invalid.", fileName.c_str(), i);
			return false;
		}

		const rapidjson::Value& rot = bindpose["rot"];
		const rapidjson::Value& trans = bindpose["trans"];

		if (!rot.IsArray() || !trans.IsArray())
		{
			SDL_Log("Skeleton %s: Bone %d is invalid.", fileName.c_str(), i);
			return false;
		}

		temp.localBindPose.mRotation.x = rot[0].GetDouble();
		temp.localBindPose.mRotation.y = rot[1].GetDouble();
		temp.localBindPose.mRotation.z = rot[2].GetDouble();
		temp.localBindPose.mRotation.w = rot[3].GetDouble();
			 
		temp.localBindPose.mTranslation.x = trans[0].GetDouble();
		temp.localBindPose.mTranslation.y = trans[1].GetDouble();
		temp.localBindPose.mTranslation.z = trans[2].GetDouble();

		this->bones.emplace_back(temp);
	}

	// Now that we have the bones
	ComputeGlobalInverseBindPose();

	return true;
}

int Skeleton::GetNumBones()
{
	return bones.size();
}

const Bone & Skeleton::GetBone(int index)
{
	return bones[index];
}

const std::vector<Bone>& Skeleton::GetBones() const
{
	return bones;
}

const std::vector<Matrix4>& Skeleton::GetGlobalInverseBindPose()
{
	return globalBindInversePose;
}

void Skeleton::ComputeGlobalInverseBindPose()
{
	//resize the number of bones
	globalBindInversePose.resize(GetNumBones());

	//compute global bind pose of each pose

	//the global bind pose of the bone is just the bind pose of the root
	globalBindInversePose[0] = bones[0].localBindPose.ToMatrix();

	//each remaining bind pose is the bind pose time's the parent's bind pose
	for (size_t i = 1; i < globalBindInversePose.size(); i++)
	{
		Matrix4 localMat = bones[i].localBindPose.ToMatrix();
		globalBindInversePose[i] = localMat * globalBindInversePose[bones[i].parentIndex];
	}

	//inverse each matrix
	for (size_t i = 0; i < globalBindInversePose.size(); i++)
	{
		globalBindInversePose[i].Invert();
	}
}
