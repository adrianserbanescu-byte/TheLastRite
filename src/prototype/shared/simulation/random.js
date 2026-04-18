export function createSeededRandom(seed = Date.now()) {
  let value = Math.abs(Math.floor(seed)) || 1;

  return () => {
    value = (value * 16807) % 2147483647;
    return (value - 1) / 2147483646;
  };
}

export function pickOne(items, random) {
  return items[Math.floor(random() * items.length)];
}

