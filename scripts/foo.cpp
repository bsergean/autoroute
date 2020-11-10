
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

    return 1;
}
