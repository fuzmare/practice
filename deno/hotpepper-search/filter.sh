jq '[ 
  .[] | select(.male_all_age >= 100) | {
    name: .name,
    ratio: (
      if (.female_teen + .female_twenty + .female_thirty + .female_forty + .female_fifty_plus) == 0 
      then 0 
      else .male_all_age / (.female_teen + .female_twenty + .female_thirty + .female_forty + .female_fifty_plus) 
      end
    )
  }
] | sort_by(.ratio) | reverse' data.json
