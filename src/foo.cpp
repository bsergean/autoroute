
class FeatureSettings
{
public:
    FeatureSettings() = default;
    bool __isFeatureEnabled(const char* feature) const { return false; }
};

class Context
{
public:
    Context() = default;
    const FeatureSettings& featureSettings() const { return _featureSettings; }
private:
    FeatureSettings _featureSettings;
};

#define AL_ENABLED( name, context ) \
    if (context.featureSettings().__isFeatureEnabled((name )))

int main()
{
    Context context;

    AL_ENABLED("foo_feature", context)
    {
        return 0;
    }
    else
    {
        return 1;
    }

    AL_ENABLED("bar_feature", context)
    {
        return 2;
    }
    else
    {
        return 3;
    }

    AL_ENABLED("baz_feature", context)
    {
        return 4;
    }
    else
    {
        return 5;
    }
}
