from __future__ import annotations

import random
from typing import Dict

from temporalio import activity


@activity.defn
async def l0_test(proxy_address: str) -> Dict[str, object]:
    """Cheap connectivity test. Replace with your real HTTP/SOCKS probe."""
    passed = not proxy_address.endswith(":0")
    latency_ms = random.randint(40, 180)
    return {
        "level": "L0",
        "passed": passed,
        "latency_ms": latency_ms,
        "detail": "basic connect ok" if passed else "invalid port or connect failed",
    }


@activity.defn
async def l1_test(proxy_address: str) -> Dict[str, object]:
    """Higher-cost test. Replace with geo / TLS / target-site checks."""
    latency_ms = random.randint(80, 260)
    passed = latency_ms < 220
    return {
        "level": "L1",
        "passed": passed,
        "latency_ms": latency_ms,
        "detail": "upstream reachable" if passed else "upstream timeout",
    }


@activity.defn
async def score_proxy(results: Dict[str, Dict[str, object]]) -> Dict[str, object]:
    score = 0
    for _, item in results.items():
        if item["passed"]:
            score += 50
            score += max(0, 200 - int(item["latency_ms"])) // 10
    return {
        "score": score,
        "grade": "A" if score >= 90 else "B" if score >= 70 else "C",
    }
