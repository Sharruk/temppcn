import json
import urllib.request
import urllib.error
from pathlib import Path

# =============== CONFIGURATION ===============

# Paste your Google Gemini API key here.
GEMINI_API_KEY = "AIzaSyA_sWp-5HgqRwZCuIe0htq8lvk5cgViJIg"

MODEL = "gemini-2.5-flash"

# Put your full lab question here.
PROMPT = r"""

i need c programs simple pls (only c files must be there...server and client.c)

1. implement crc generation at sender using configurable generator polynomials (1011,1101,1001) and append crc bits to sensor data
2. simpulate transimission over communication channel liek client server by introducing random bit errors into transmitted frames
3. implement reciever side crc verification and display error messages indicating wherer each frame is error free or corrupted
4. extend app to support multiple sensor nodes like multi client tcp shit and assign sequence numbers to transmitted frame and log transmission details, detected errors,crc verification results
5. evealuate the error detection perfeomance using different generator polynomials and error patterns

let liek the main data be to be hardcoded into the code liek "10101" (or i can put up my own value liek 110011) and the generator
array to like containing all 3 polynomial generators together and program iterates and like calulates iteratively thru each of the 
generator thru like ./client 1 runs thru all 3 poly for node 1....

(pls gimme the file codes pls...no comments and make it like simple and human written so my mam wont sus )
"""

SYSTEM_INSTRUCTION = """
You are a coding assistant for college practical exams.
Give a simple, complete solution with folder structure and full code
for every required file. Do not use placeholders or omit important code.
Include dependencies, exact run commands, and a short explanation.
State version assumptions. Do not claim code was tested unless it was.
When asked to continue, continue exactly from where the previous answer
stopped. Do not repeat completed sections or restart the answer.
"""

OUTPUT_FILE = "output.md"


def call_gemini(url, api_key, contents):
    payload = {
        "system_instruction": {"parts": [{"text": SYSTEM_INSTRUCTION}]},
        "contents": contents,
        "generationConfig": {
            "temperature": 0.2
        }
    }

    request = urllib.request.Request(
        url,
        data=json.dumps(payload).encode("utf-8"),
        headers={
            "Content-Type": "application/json",
            "x-goog-api-key": api_key
        },
        method="POST"
    )

    with urllib.request.urlopen(request, timeout=240) as response:
        result = json.loads(response.read().decode("utf-8"))

    candidates = result.get("candidates", [])
    if not candidates:
        raise RuntimeError("Gemini returned no candidates: " + json.dumps(result))

    candidate = candidates[0]
    parts = candidate.get("content", {}).get("parts", [])
    answer = "".join(part.get("text", "") for part in parts if "text" in part)
    finish_reason = candidate.get("finishReason", "UNKNOWN")
    return answer, finish_reason, candidate.get("content")


def main():
    api_key = GEMINI_API_KEY.strip()

    if not api_key or api_key == "PASTE_YOUR_GEMINI_API_KEY_HERE":
        print("ERROR: Put your API key in GEMINI_API_KEY at the top of this file.")
        return

    url = (
        "https://generativelanguage.googleapis.com/v1beta/models/"
        + MODEL
        + ":generateContent"
    )

    contents = [{
        "role": "user",
        "parts": [{"text": PROMPT}]
    }]

    full_answer = ""
    Path(OUTPUT_FILE).write_text("", encoding="utf-8")

    try:
        attempt = 0
        while True:
            attempt += 1
            if attempt == 1:
                print(f"Calling Google Gemini ({MODEL})...")
            else:
                print(f"\\n[Continuing response, part {attempt}...]")

            answer, finish_reason, model_content = call_gemini(url, api_key, contents)

            if answer:
                full_answer += answer
                with open(OUTPUT_FILE, "a", encoding="utf-8") as output:
                    output.write(answer)
                    output.flush()
                print(answer, end="", flush=True)

            if model_content:
                contents.append(model_content)

            if finish_reason != "MAX_TOKENS":
                print("\\n\\n[Generation finished.]")
                break

            # No application-level continuation cap. Continue until Gemini
            # finishes normally or the API returns an error.
            contents.append({
                "role": "user",
                "parts": [{
                    "text": (
                        "Continue exactly where your previous response stopped. "
                        "Do not repeat text or code already provided. Finish the "
                        "current file/section, then provide the remaining required files "
                        "and instructions."
                    )
                }]
            })

        print(f"\\nSaved answer to: {Path(OUTPUT_FILE).resolve()}")

    except urllib.error.HTTPError as error:
        details = error.read().decode("utf-8", errors="replace")
        print(f"\nHTTP error {error.code}:\n{details}")
        print("Check API key, model access, quota, and request limits.")
    except urllib.error.URLError as error:
        print(f"\nConnection error: {error.reason}")
        print("Check internet access and whether the lab network allows Google's API.")
    except TimeoutError:
        print("\nRequest timed out. Try again or shorten the prompt.")
    except Exception as error:
        print(f"\nError: {error}")


if __name__ == "__main__":
    main()
