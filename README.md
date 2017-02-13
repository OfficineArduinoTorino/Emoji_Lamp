# Emoji Lamps
![The lamp](/images/EmojiLampFoto.png)

## Description

The emoji-lamp is a smart lamp you can chat with. It will react with different colors depending on the emoji you sent.
It can also be used with another lamp, to make them communicate with each other.

Pressing the white button it will behave as a normal lamp, but when you press the other buttons that's when the magic happen.

Each colored button is associated to a different emoji and a different light color. When you press one of them the lamp will send an emoji to the chat you chose and the other lamp will light up with the right color.

## Getting started

#### Understanding chatbots

A chatbot is a service, powered by rules and sometimes artificial intelligence, that you interact with via a chat interface. The service could be any number of things, ranging from functional to fun, and it could live in any major chat product (Facebook Messenger, Slack, Telegram, Text Messages, etc.). In this project we are going to use a Telegram bot.

#### Creating your bot

To generate your new Bot, you need an Access Token. Open Telegram, talk to [BotFather](https://telegram.me/botfather) and follow these few simple steps.

![Step 1](/images/Bot_step_1.jpeg)

![Step 1](/images/Bot_step_2.jpeg)

![Step 1](/images/Bot_step_3.jpeg)

![Step 1](/images/Bot_step_4.jpeg)

![Step 1](/images/Bot_step_5.jpeg)

Save the token you just received, we will use it later.

In order to allow your bot to read the emoji (and not only the commands) you have to disable the privacy mode. Talk to BotFather and use this commands:

```
/mybots
@yourBot
Bot setting
Group privacy
Turn off
```
#### Wiring up the lamp

![Pin Mapping](/images/PinMapping.png)

#### Connect the lamp

Once you have plugged the lamp in the outlet, you should see in the available networks of your WiFi the network "Your_Emoji_Lamp".

Connect to the network, a browser page should pop up. If not, open the browser and try to reach whatever website, you will be redirect to the right page.

![The page](/images/ScreenOne.png)

Connect to your WiFi and insert the token of your bot.
Wait for a few seconds and you are done!

WARNING: before pressing any button send a text message to your bot. It will give to the lamp the chat id, such that the lamp will know to what chat send the messages.

Now you can add the bot to the chat you want or directly talk to it.

#### Resetting the lamp

If you want to reset some parameters of the lamp press and hold the white button untill you see the white lights quickly flashing for three times. It will erase all the credentials and return online as "Your_Emoji_Lamp" network.



## Hardware
Hardware :

* Olimex ESP8266-EVB ([Documentation here](https://www.olimex.com/Products/IoT/ESP8266-EVB/open-source-hardware))
* Neopixel rgb ws2812 x2
* Pushbutton x4
