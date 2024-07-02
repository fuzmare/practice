interface Fingers {
  index: string[];
  middle: string[];
  ring: string[];
  pinky: string[];
}

class Hand {
  private lastFinger: keyof Fingers;
  private fingers: Fingers;

  constructor() {
    this.lastFinger = "index";
    this.fingers = {
      index: [],
      middle: [],
      ring: [],
      pinky: [],
    };
  }

  addKeyToFinger(finger: keyof Fingers, keys: string[]): void {
    this.fingers[finger] = this.fingers[finger].concat(keys);
  }

  setLastFinger(finger: keyof Fingers): void {
    this.lastFinger = finger;
  }

  getLastFinger(): keyof Fingers {
    return this.lastFinger;
  }

  getKeysForFinger(finger: keyof Fingers): string[] {
    this.setLastFinger(finger);
    return this.fingers[finger];
  }

  getRandomKeyAvoidingLastFinger(): string | null {
    const availableFingers: (keyof Fingers)[] =
      (Object.keys(this.fingers) as (keyof Fingers)[])
        .filter((finger) => finger !== this.lastFinger);

    const randomFinger =
      availableFingers[Math.floor(Math.random() * availableFingers.length)];
    const keysForFinger = this.fingers[randomFinger];

    if (keysForFinger.length === 0) {
      return null;
    }

    const randomKey =
      keysForFinger[Math.floor(Math.random() * keysForFinger.length)];
    this.setLastFinger(randomFinger);
    return randomKey;
  }
}

export default Hand;

function typeAlternatingKeys(
  leftHand: Hand,
  rightHand: Hand,
  typeCount: number,
): string {
  let result = "";
  let currentHand = leftHand;

  for (let i = 0; i < typeCount; i++) {
    const key = currentHand.getRandomKeyAvoidingLastFinger();

    if (key !== null) {
      result += key;
    }

    // 手を交互に切り替える
    currentHand = (currentHand === leftHand) ? rightHand : leftHand;
  }

  return result;
}

function randomizeCase(str: string, upperCaseProbability: number): string {
  return str.split("").map((char) => {
    return Math.random() < upperCaseProbability ? char.toUpperCase() : char;
  }).join("");
}

function insertSpacesAtInterval(text: string, interval: number): string {
  if (interval <= 0) {
    throw new Error("Interval must be greater than 0");
  }

  let result = "";
  for (let i = 0; i < text.length; i += interval) {
    if (i + interval < text.length) {
      result += text.substr(i, interval) + " ";
    } else {
      result += text.substr(i);
    }
  }

  return result.trim();
}

const left = new Hand();
const right = new Hand();

left.addKeyToFinger("pinky", ["q", "a", "a", "1"]);
left.addKeyToFinger("ring", ["w", "s", "s", "2", "3"]);
left.addKeyToFinger("middle", ["e", "d", "d", "4"]);
left.addKeyToFinger("index", ["r", "f", "f", "4", "5", "6"]);

right.addKeyToFinger("index", ["u", "j", "j", "7"]);
right.addKeyToFinger("middle", ["i", "k", "k", "8"]);
right.addKeyToFinger("ring", ["o", "l", "l", "9"]);
right.addKeyToFinger("pinky", ["p", "@", "0"]);

const randomString = typeAlternatingKeys(left, right, 20);
console.log(insertSpacesAtInterval(randomString,5));
