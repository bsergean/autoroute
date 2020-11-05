#define AL_ENABLED

int main()
{
    AL_ENABLED("bar_feature", context)
    {
        return 0;
    }
    else
    {
        return 1;
    }
}
