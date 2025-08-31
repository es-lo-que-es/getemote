import requests as req
import sys, json

CLIENT_ID = ""
CLIENT_SECRET = ""
CHANNEL_NAME = "twitch"


#                       HANDLE INPUT
try:
    CLIENT_ID = open("../tokens/CLIENT_ID", "r").readline().rstrip('\n')
    CLIENT_SECRET = open("../tokens/CLIENT_SECRET", "r").readline().rstrip('\n')
except Exception as e:
    print("failed to read credentials from ../tokens/: ", e)
    exit(-1)


# default to twitch official channel if no username was provided
if len(sys.argv) > 1: CHANNEL_NAME = sys.argv[1]


#                       GET AUTH TOKEN
token_req_url = "https://id.twitch.tv/oauth2/token"
token_req_headers = { "Content-Type": "application/x-www-form-urlencoded" }

token_req_data = {
    "grant_type": "client_credentials",
    "client_secret": CLIENT_SECRET,
    "client_id": CLIENT_ID,
}

resp = req.post(token_req_url, headers=token_req_headers, data=token_req_data)

if resp.status_code != 200: 
    print("auth request failed: ", resp.reason)
    exit(-1)

TOKEN = json.loads(resp.text)['access_token']
print("AUTH TOKEN RECEIVED")

#                       RETRIEVE BROADCASTER ID FOR THE GIVEN CHANNEL
broadcaster_req_url = f"https://api.twitch.tv/helix/users?login={CHANNEL_NAME}"

auth_headers = {
    "Authorization": f"Bearer {TOKEN}",
    "Client-Id": CLIENT_ID,
}

resp = req.get(broadcaster_req_url, headers=auth_headers)

if resp.status_code != 200:
    print("id request failed: ", resp.reason)
    exit(-1)

BROADCASTER_ID = json.loads(resp.text)['data'][0]['id']
print("CHANNEL_ID:", BROADCASTER_ID)


#                       RETRIEVE CHANNEL EMOTES LIST
emotes_req_url = f"https://api.twitch.tv/helix/chat/emotes?broadcaster_id={BROADCASTER_ID}"
resp = req.get(emotes_req_url, headers=auth_headers)

if resp.status_code != 200:
    print("emotes request failed: ", resp.reason)
    exit(-1)

emotes = json.loads(resp.text)['data']
result = []


for entry in emotes:

    name = entry['name']
    url = entry['images']['url_4x']
    format = entry['format']

    result.append((name, url, format))


for emote in result: print(emote)
