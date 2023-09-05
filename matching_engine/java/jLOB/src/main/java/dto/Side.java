package dto;


import com.google.common.base.Preconditions;

import java.util.List;

public enum Side {

    BID,OFFER;

    public static Side fromString(String side) {
        String resultSide = side.toUpperCase();
        Preconditions.checkArgument(List.of("BID", "OFFER").contains(resultSide));
        return resultSide.equals("BID") ? Side.BID : Side.OFFER;
    }
}
