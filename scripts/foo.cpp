#define AL_ENABLED(a, b) if (false)

int main()
{
    AL_ENABLED("bar_feature", "context")
    {
        return 0;
    }
    else
    {
        return 1;
    }
}
