/*
 *  Singleton.h
 *
 *  Created by Nan Zhang on 7/18/12, zhang.nan@epa.gov
 *
 */


#ifndef INC_SINGLETON_H_
#define INC_SINGLETON_H_
	 
template <class T>
class Singleton
{
public:
    static T* getInstance() 
    {
        if(!m_pInstance) m_pInstance = new T;
	    return m_pInstance;
    }
	
protected:
    Singleton();
    ~Singleton();

private:
    Singleton(Singleton const&);
    Singleton& operator=(Singleton const&);
    static T* m_pInstance;
};
	 
template <class T> T* Singleton<T>::m_pInstance=NULL;
	 

#endif
