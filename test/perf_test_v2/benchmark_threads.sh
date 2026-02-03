#!/bin/bash
set -euo pipefail

SCRIPT_DIR="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"
prog="$SCRIPT_DIR/../../build/main"

indir="./test/performance_test/data/spamassassin"
regex_all="./test/perf_test_v2/benchmark/easy_1000.rgx"

# parametry testu
WARMUP=2
ITER=3

REGEXES=1000

# ile WĄTKÓW testować
STEPS=(1 2 3 4 5 10 15 30 40 50 60 80 90 100 150 200 250 300 400 450 500 550 600 650 700 800 900 1000)

out_csv="$SCRIPT_DIR/bench_results_threads_easy.csv"
echo "threads,engine,avg_ms,raw_line" > "$out_csv"

tmp_rgx="$(mktemp)"
trap 'rm -f "$tmp_rgx"' EXIT

extract_avg_ms() {
  awk 'match($0,/avg=[ \t]*[0-9]+(\.[0-9]+)?/) { s=substr($0,RSTART,RLENGTH); sub(/avg=[ \t]*/,"",s); print s }' <<<"$1"
}


# zbuduj plik z pierwszymi REGEXES regexami (raz)
head -n "$REGEXES" "$regex_all" > "$tmp_rgx"

run_one() {
  local threads="$1"
  local engine="$2"

  

  # uruchom i wyciągnij linię z avg=
  local line
  line="$("$prog" -f "$indir" -r "$tmp_rgx" -e "$engine" -w "$WARMUP" -t "$ITER" -h "$threads" | grep -m1 "avg=" || true)"

  if [[ -z "$line" ]]; then
    echo "WARN: brak avg= dla engine=$engine threads=$threads" >&2
    echo "$threads,$engine,," >> "$out_csv"
    return
  fi

  local avg
  avg="$(extract_avg_ms "$line")"

  echo "$threads,$engine,$avg,\"$line\"" >> "$out_csv"
}

for n in "${STEPS[@]}"; do
  echo "== threads=$n (regex_count=$REGEXES) ==" >&2
  run_one "$n" "hs"
  run_one "$n" "pcre"
done

echo "Zapisano: $out_csv"
