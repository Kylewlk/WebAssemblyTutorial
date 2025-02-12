#include <string>


class Context
{
public:
    static Context* getInstance();

    void setUrl(const char* url)
    {
        this->url = url;
    }

    const char* getUrl() const
    {
        return this->url.c_str();
    }

    const std::string& getUrlString() const
    {
        return this->url;
    }

private:
    Context() = default;

    std::string url;
};
