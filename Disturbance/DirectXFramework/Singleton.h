#pragma once

template <class T>
class Singleton
{
public :
	static T* GetInstance()
	{
		if (!mInstance)
		{
			mbInitialized = true;
			mInstance = new T;
		}
		assert(mInstance != NULL);
		return mInstance;
	}

	static void DestroyInstance()
	{
		if (mInstance)
		{
			delete mInstance;
			mInstance = NULL;
		}
	}

	static bool mbInitialized;

protected:
	Singleton()
	{
		assert(!mInstance);
		assert(mbInitialized);
	};
	virtual ~Singleton() = default;
private:
	static T* mInstance;
	Singleton(Singleton const&);
	Singleton& operator=(Singleton const&);
};

template <class T>
T* Singleton<T>::mInstance = nullptr;
template <class T>
bool Singleton<T>::mbInitialized = false;
