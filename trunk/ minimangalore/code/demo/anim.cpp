

//-----------------------------------------------------------------------------
/**
   �ش� �ִϸ��̼� Ŭ���� ������ Ű�������� nAnimation::Group ����.

   nAnimation::Group::TimeToIndex()

   ���� Ű ������ : 0
   ������ Ű ������ : numKeys - 1

   �ش� ���ϸ��̼��� clamp Ÿ���� ���, ������ �ð��� ���� Ű�������� 
   ������ Ű �������� �ʰ��ϸ� ������ Ű�� �����ȴ�.
*/
matrix44 
nCharacter2::PredictJointTM(const nString& clipName, const nString& jointName, 
							 float startTime, float endTime)
{
	nAnimStateInfo animStateInfo;


	animStateInfo.SetStateStarted(startTime);
	animStateInfo.SetStateOffset();

    animStateInfo.SetFadeInTime(characterSet->GetFadeInTime());
    animStateInfo.BeginClips(1);
	
	int clipIndex = this->skinAnimator->GetClipIndexByName(clipName);

	//get animation group first.
	const nAnimClip& animClip = this->skinAnimator->GetClipAt(clipIndex);

    float weightSum = 0.0f;
	weightSum += characterSet->GetClipWeightAt(0);

	animStateInfo.SetClip(0, GetClipAt(index, animClip, characterSet->GetClipWeightAt(i) / weightSum);
	animStateInfo.EndClips();



	int jointIndex = this->characterSkeleton.GetJointIndexByName(jointName);

    vector3 translate, prevTranslate;
    quaternion rotate, prevRotate;
    vector3 scale, prevScale;

	matrix44 ret;

    float curRelTime = endTime - animStateInfo.GetStateStarted();

    // handle time exception (this happens when time is reset to a smaller value
    // since the last animation state switch)
    if (curRelTime < 0.0f)
    {
        curRelTime = 0.0f;
        animStateInfo.SetStateStarted(endTime);
    }

    float sampleTime = curRelTime + animStateInfo.GetStateOffset();

    if (this->Sample(animStateInfo, sampleTime, nCharacter2::keyArray, nCharacter2::scratchKeyArray, nCharacter2::MaxCurves))
    {
	    const vector4* keyPtr = keyArray;

		keyPtr += jointIndex;

	    // read sampled translation, rotation and scale
		translate.set(keyPtr->x, keyPtr->y, keyPtr->z);          keyPtr++;
		rotate.set(keyPtr->x, keyPtr->y, keyPtr->z, keyPtr->w);  keyPtr++;
		scale.set(keyPtr->x, keyPtr->y, keyPtr->z);              keyPtr++;

		ret.translate(translate);
		ret.set(rotate);
		ret.sacle(scale);
	}

	return ret;
}
