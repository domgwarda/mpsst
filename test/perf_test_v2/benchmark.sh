#!/bin/bash
set -euo pipefail

SCRIPT_DIR="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"
prog="$SCRIPT_DIR/../../build/main"

indir="./test/performance_test/data/spamassassin"
regex_all="./test/perf_test_v2/benchmark/words_1000.rgx"

# parametry testu
WARMUP=3
ITER=7

# ile regexów testować (dostosuj do pliku)
STEPS=(1 2 5 10 20 50 100 200 500 1000)

out_csv="$SCRIPT_DIR/benchmark/bench_results.csv"
echo "regex_count,engine,avg_ms,raw_line" > "$out_csv"

tmp_rgx="$(mktemp)"
trap 'rm -f "$tmp_rgx"' EXIT

extract_avg_ms() {
  # weź pierwszą liczbę po avg= (np. avg=12.34ms albo avg=12.34)
  # dopasuj jeśli masz inny format
  echo "$1" | sed -nE 's/.*avg=([0-9]+([.][0-9]+)?).*/\1/p'
}

run_one() {
  local count="$1"
  local engine="$2"

  # zbuduj plik z pierwszymi N regexami
  head -n "$count" "$regex_all" > "$tmp_rgx"

  # uruchom i wyciągnij linię z avg=
  local line
  line="$("$prog" -f "$indir" -r "$tmp_rgx" -e "$engine" -w "$WARMUP" -t "$ITER"  | grep -m1 "avg=" || true)"

  if [[ -z "$line" ]]; then
    echo "WARN: brak avg= dla engine=$engine count=$count" >&2
    echo "$count,$engine,," >> "$out_csv"
    return
  fi

  local avg
  avg="$(extract_avg_ms "$line")"

  # jeśli parsowanie nie wyszło, też zapisz surową linię
  echo "$count,$engine,$avg,\"$line\"" >> "$out_csv"
}

for n in "${STEPS[@]}"; do
  echo "== regex_count=$n ==" >&2
  run_one "$n" "hs"
  run_one "$n" "pcre"
done
