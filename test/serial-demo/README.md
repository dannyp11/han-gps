# Where is the important stuff
In `monitor.c`, there is a monitor thread, which prints a message on both USART1 and software serial, and prompts to enter a character from the software serial, which will be echoed on both ports.

# Pinouts
Rx: PD2
Tx: PD3

# BAUD Settings
In this example, USART1 is set at B4800, software serial is set at B9600. 

```
sdStart(&SD1, NULL);
sdStart(&SDS, &softserial_config);
```

The above code should be self documenting. Notice that these only needed to be called once on bootup. BAUD and such are set in the second parameter, where NULL means default.

Software serial doesn't have a default.

# Explanation
`SD1` is the driver for USART1, `SDS` is that of software serial. To print a message, use `chprintf(BaseSequentialStream *, ...)`.

`BaseSequentialStream *` is casted directly from the driver pointer, e.g. `(BaseSequentialStream *) &SDS`. The software driver is fully compatible with any ChibiOS features, so it can be used as a channel, event source, etc.

# If it doesn't work?
Find Rongcui. I did not test this demo when I stripped it down from the trunk. It compiles, so it probably works.