from flask import Flask, request
import base64
from io import BytesIO
from PIL import Image
import requests

api_key = "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";

app = Flask(__name__)

@app.route("/", methods=["POST"])
def hello_world():
    image_b64 = request.json["image"]

    image_io = BytesIO(base64.b64decode(image_b64))
    image_pil = Image.open(image_io)
    image_pil.save("out.jpeg");

    headers = {
        "Content-Type": "application/json",
        "Authorization": f"Bearer {api_key}"
    }

    payload = {
        "model": "gpt-4-vision-preview",
        "messages": [
            {
                "role": "user",
                "content": [
                    {
                        "type": "text",
                        "text": "What is in this image?"
                    },
                    {
                    "type": "image_url",
                        "image_url": {
                            "url": f"data:image/jpeg;base64,{image_b64}"
                        }
                    }
                ]
            }
        ],
        "max_tokens": 1024
    }

    response = requests.post("https://api.openai.com/v1/chat/completions", headers=headers, json=payload)
    response = response.json() 

    print(response)

    return response

if __name__ == "__main__":
    app.run("0.0.0.0", 5000)