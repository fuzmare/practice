import { DOMParser } from "https://deno.land/x/deno_dom/deno-dom-wasm.ts";

async function fetchText(url: string): Promise<string> {
  const response = await fetch(url);
  const text = await response.text();
  return text;
}

async function scrapeSalons(url: string) {
  const html = await fetchText(url);
  const document = new DOMParser().parseFromString(html, "text/html");

  if (document) {
    const salons = document.querySelectorAll(".slnCassetteList > li");
    
    for (const salon of salons) {
      // サロンの名前を取得
      const salonNameElement = salon.querySelector(".slnName a");
      const salonName = salonNameElement?.textContent || "Unknown Salon";
      console.log(`Scraping reviews for salon: ${salonName}`);

      // 口コミページのリンクを取得
      const reviewLink = salon.querySelector(".message a")?.getAttribute("href");
      if (reviewLink) {
        await scrapeReviews(reviewLink);
      }
    }
    
    // 次のページへのリンクをチェック
    const nextPageLink = document.querySelector(".postList .afterPage a")?.getAttribute("href");
    if (nextPageLink) {
      console.log(`Moving to next page: ${nextPageLink}`);
      await scrapeSalons(nextPageLink); // 次のページに対して同じ関数を再帰的に実行
    } else {
      console.log("No more pages to scrape.");
    }
  }
}

async function scrapeReviews(url: string) {
  const html = await fetchText(url);
  const document = new DOMParser().parseFromString(html, "text/html");

  if (document) {
    const reviewCounts = document.querySelectorAll(".reviewFilterAgeList .jscReviewFilterReviewCount");

    const reviewData = {
      female_teen: parseInt(reviewCounts[0]?.textContent || "0", 10),
      female_twenty: parseInt(reviewCounts[1]?.textContent || "0", 10),
      female_thirty: parseInt(reviewCounts[2]?.textContent || "0", 10),
      female_forty: parseInt(reviewCounts[3]?.textContent || "0", 10),
      female_fifty_plus: parseInt(reviewCounts[4]?.textContent || "0", 10),
      male_all_age: parseInt(reviewCounts[5]?.textContent || "0", 10)
    };

    console.log(reviewData);
    // TODO: データベースにレビューデータを保存する処理をここに実装する
  }
}

// 最初のページからスクレイピングを開始
const startUrl = "https://beauty.hotpepper.jp/CSP/bt/stationSalonSearch/condtion/?ssk=3&sortType=popular&serviceAreaCd=SA&stationCd=3110070&changeSta=0&searchGender=ALL&pn=1";
scrapeSalons(startUrl);
