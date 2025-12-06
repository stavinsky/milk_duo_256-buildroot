#include <linux/module.h>
#include <linux/platform_device.h>
#include <sound/soc.h>

static struct snd_soc_dai_driver dummy_dai = {
    .name = "dummy-hifi",
    .playback = {
        .stream_name = "Playback",
        .channels_min = 1,
        .channels_max = 2,
        .rates = SNDRV_PCM_RATE_8000_192000,
        .formats = SNDRV_PCM_FMTBIT_S16_LE |
                   SNDRV_PCM_FMTBIT_S24_LE,
    },
};

static const struct snd_soc_component_driver dummy_component = {
    .name = "dummy-codec",
};

static int dummy_codec_probe(struct platform_device* pdev) {
    int ret;

    dev_info(&pdev->dev, "dummy codec probe()\n");

    ret = devm_snd_soc_register_component(&pdev->dev,
                                          &dummy_component,
                                          &dummy_dai, 1);
    if (ret)
        dev_err(&pdev->dev, "register component failed: %d\n", ret);

    return ret;
}

static const struct of_device_id dummy_of_match[] = {
    {
        .compatible = "sg2002,dummy-codec",
    },
    {}};
MODULE_DEVICE_TABLE(of, dummy_of_match);

static struct platform_driver dummy_codec_driver = {
    .driver = {
        .name = "sg2002-dummy-codec",
        .of_match_table = dummy_of_match,
    },
    .probe = dummy_codec_probe,
};
module_platform_driver(dummy_codec_driver);

MODULE_LICENSE("GPL");
MODULE_DESCRIPTION("dummy soc codec");