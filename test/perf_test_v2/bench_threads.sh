#!/bin/bash
set -euo pipefail

SCRIPT_DIR="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"
prog="$SCRIPT_DIR/../../build/main"

indir="./test/performance_test/data/spamassassin"
regex_all="./test/perf_test_v2/benchmark/normal_1000.rgx"   # albo hard_1000.rgx

# parametry testu
WARMUP=5
ITER=15

# oś X: ile regexów
REGEX_STEPS=(1 2 5 10 20 50 100 200 500 1000)

# linie: ile wątków (WYBIERZ MNIEJ niż miałeś w pełnym zakresie)
THREAD_STEPS=(1 2 4 8 16 32 64 128 256)   # dopasuj pod CPU / sensowny zakres

out_csv="$SCRIPT_DIR/benchmark/bench_matrix.csv"
mkdir -p "$(dirname "$out_csv")"
echo "regex_count,threads,engine,avg_ms,raw_line" > "$out_csv"

tmp_rgx="$(mktemp)"
trap 'rm -f "$tmp_rgx"' EXIT

extract_avg_ms() {
  # wyciąga liczbę z "avg= 12.34" / "avg=12.34ms" etc.
  awk 'match($0,/avg=[ \t]*[0-9]+(\.[0-9]+)?/) {
         s=substr($0,RSTART,RLENGTH);
         sub(/avg=[ \t]*/,"",s);
         print s
       }' <<<"$1"
}

run_one() {
  local count="$1"
  local threads="$2"
  local engine="$3"

  head -n "$count" "$regex_all" > "$tmp_rgx"

  local line
  line="$("$prog" -f "$indir" -r "$tmp_rgx" -e "$engine" -w "$WARMUP" -t "$ITER" -h "$threads" \
        | grep -m1 "avg=" || true)"

  if [[ -z "$line" ]]; then
    echo "WARN: brak avg= dla engine=$engine threads=$threads count=$count" >&2
    echo "$count,$threads,$engine,," >> "$out_csv"
    return
  fi

  local avg
  avg="$(extract_avg_ms "$line")"

  echo "$count,$threads,$engine,$avg,\"$line\"" >> "$out_csv"
}

for threads in "${THREAD_STEPS[@]}"; do
  echo "==== threads=$threads ====" >&2
  for n in "${REGEX_STEPS[@]}"; do
    echo "  == regex_count=$n ==" >&2
    run_one "$n" "$threads" "hs"
    run_one "$n" "$threads" "pcre"
  done
done

echo "Zapisano: $out_csv"
